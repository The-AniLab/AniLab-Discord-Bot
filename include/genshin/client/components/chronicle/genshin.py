"""Genshin battle chronicle component."""

import asyncio
import functools
import typing

from genshin import errors, paginators, types, utility
from genshin.models.genshin import character as character_models
from genshin.models.genshin import chronicle as models

from . import base

__all__ = ["GenshinBattleChronicleClient"]


class GenshinBattleChronicleClient(base.BaseBattleChronicleClient):
    """Genshin battle chronicle component."""

    async def _request_genshin_record(
        self,
        endpoint: str,
        uid: typing.Optional[int] = None,
        *,
        method: str = "GET",
        lang: typing.Optional[str] = None,
        payload: typing.Optional[typing.Mapping[str, typing.Any]] = None,
        cache: bool = True,
    ) -> typing.Mapping[str, typing.Any]:
        """Get an arbitrary honkai object."""
        payload = dict(payload or {})
        original_payload = payload.copy()

        uid = uid or await self._get_uid(types.Game.GENSHIN)
        payload = dict(role_id=uid, server=utility.recognize_genshin_server(uid), **payload)

        data, params = None, None
        if method == "POST":
            data = payload
        else:
            params = payload

        cache_key: typing.Optional[base.ChronicleCacheKey] = None
        if cache:
            cache_key = base.ChronicleCacheKey(
                types.Game.GENSHIN,
                endpoint,
                uid,
                lang=lang or self.lang,
                params=tuple(original_payload.values()),
            )

        return await self.request_game_record(
            endpoint,
            lang=lang,
            game=types.Game.GENSHIN,
            region=utility.recognize_region(uid, game=types.Game.GENSHIN),
            params=params,
            data=data,
            cache=cache_key,
        )

    async def get_partial_genshin_user(
        self,
        uid: int,
        *,
        lang: typing.Optional[str] = None,
    ) -> models.PartialGenshinUserStats:
        """Get partial genshin user without character equipment."""
        data = await self._request_genshin_record("index", uid, lang=lang)
        return models.PartialGenshinUserStats(**data)

    async def get_genshin_characters(
        self,
        uid: int,
        *,
        lang: typing.Optional[str] = None,
    ) -> typing.Sequence[models.Character]:
        """Get genshin user characters."""
        data = await self._request_genshin_record("character", uid, lang=lang, method="POST")
        return [models.Character(**i) for i in data["avatars"]]

    async def get_genshin_user(
        self,
        uid: int,
        *,
        lang: typing.Optional[str] = None,
    ) -> models.GenshinUserStats:
        """Get genshin user."""
        data, character_data = await asyncio.gather(
            self._request_genshin_record("index", uid, lang=lang),
            self._request_genshin_record("character", uid, lang=lang, method="POST"),
        )
        data = {**data, **character_data}

        return models.GenshinUserStats(**data)

    async def get_genshin_spiral_abyss(
        self,
        uid: int,
        *,
        previous: bool = False,
        lang: typing.Optional[str] = None,
    ) -> models.SpiralAbyss:
        """Get genshin spiral abyss runs."""
        payload = dict(schedule_type=2 if previous else 1)
        data = await self._request_genshin_record("spiralAbyss", uid, lang=lang, payload=payload)

        return models.SpiralAbyss(**data)

    async def get_genshin_notes(
        self,
        uid: typing.Optional[int] = None,
        *,
        lang: typing.Optional[str] = None,
        autoauth: bool = True,
    ) -> models.Notes:
        """Get genshin real-time notes."""
        try:
            data = await self._request_genshin_record("dailyNote", uid, lang=lang, cache=False)
        except errors.DataNotPublic as e:
            # error raised only when real-time notes are not enabled
            if uid and (await self._get_uid(types.Game.GENSHIN)) != uid:
                raise errors.GenshinException(e.response, "Cannot view real-time notes of other users.") from e
            if not autoauth:
                raise errors.GenshinException(e.response, "Real-time notes are not enabled.") from e

            await self.update_settings(3, True, game=types.Game.GENSHIN)
            data = await self._request_genshin_record("dailyNote", uid, lang=lang, cache=False)

        return models.Notes(**data)

    async def get_genshin_activities(self, uid: int, *, lang: typing.Optional[str] = None) -> models.Activities:
        """Get genshin activities."""
        data = await self._request_genshin_record("activities", uid, lang=lang)
        return models.Activities(**data)

    async def get_genshin_tcg_preview(self, uid: int, *, lang: typing.Optional[str] = None) -> models.TCGPreview:
        """Get genshin tcg."""
        data = await self._request_genshin_record("gcg/basicInfo", uid, lang=lang)
        return models.TCGPreview(**data)

    async def _get_genshin_tcg_page(
        self,
        page: int,
        *,
        uid: typing.Optional[int] = None,
        characters: bool = True,
        action: bool = True,
        limit: int = 32,
        lang: typing.Optional[str] = None,
    ) -> typing.Sequence[models.TCGBaseCard]:
        """Get genshin tcg page."""
        uid = uid or await self._get_uid(types.Game.GENSHIN)
        params = dict(
            need_avatar="true" if characters else "false",
            need_action="true" if action else "false",
            offset=(page - 1) * limit,
            limit=limit,
            need_stats="false",
        )
        data = await self._request_genshin_record("gcg/cardList", uid, lang=lang, payload=params, cache=False)
        return [
            (models.TCGCharacterCard(**i) if i["card_type"] == models.TCGCardType.CHARACTER else models.TCGCard(**i))
            for i in data["card_list"]
        ]

    def genshin_tcg(
        self,
        uid: typing.Optional[int] = None,
        *,
        limit: typing.Optional[int] = None,
        characters: bool = True,
        action: bool = True,
        page_size: int = 32,
        lang: typing.Optional[str] = None,
    ) -> paginators.PagedPaginator[models.TCGBaseCard]:
        """Get genshin tcg cards."""
        return paginators.PagedPaginator(
            functools.partial(
                self._get_genshin_tcg_page,
                uid=uid,
                characters=characters,
                action=action,
                limit=page_size,
                lang=lang,
            ),
            limit=limit,
            page_size=page_size,
        )

    async def get_full_genshin_user(
        self,
        uid: int,
        *,
        lang: typing.Optional[str] = None,
    ) -> models.FullGenshinUserStats:
        """Get a genshin user with all their possible data."""
        user, abyss1, abyss2, activities = await asyncio.gather(
            self.get_genshin_user(uid, lang=lang),
            self.get_genshin_spiral_abyss(uid, lang=lang, previous=False),
            self.get_genshin_spiral_abyss(uid, lang=lang, previous=True),
            self.get_genshin_activities(uid, lang=lang),
        )
        abyss = models.SpiralAbyssPair(current=abyss1, previous=abyss2)

        return models.FullGenshinUserStats(**user.dict(), abyss=abyss, activities=activities)

    async def set_top_genshin_characters(
        self,
        characters: typing.Sequence[types.IDOr[character_models.BaseCharacter]],
        *,
        uid: typing.Optional[int] = None,
    ) -> None:
        """Set the top 8 visible genshin characters for the current user."""
        uid = uid or await self._get_uid(types.Game.GENSHIN)

        await self.request_game_record(
            "character/top",
            game=types.Game.GENSHIN,
            region=utility.recognize_region(uid, game=types.Game.GENSHIN),
            data=dict(
                avatar_ids=[int(character) for character in characters],
                uid_key=uid,
                server_key=utility.recognize_genshin_server(uid),
            ),
        )

    get_spiral_abyss = get_genshin_spiral_abyss
    get_notes = get_genshin_notes
    get_activities = get_genshin_activities
