"""StarRail battle chronicle component."""
import asyncio
import typing

from genshin import errors, types, utility
from genshin.models.starrail import chronicle as models

from . import base

__all__ = ["StarRailBattleChronicleClient"]


class StarRailBattleChronicleClient(base.BaseBattleChronicleClient):
    """StarRail battle chronicle component."""

    async def _request_starrail_record(
        self,
        endpoint: str,
        uid: typing.Optional[int] = None,
        *,
        method: str = "GET",
        lang: typing.Optional[str] = None,
        payload: typing.Optional[typing.Mapping[str, typing.Any]] = None,
        cache: bool = True,
    ) -> typing.Mapping[str, typing.Any]:
        """Get an arbitrary starrail object."""
        payload = dict(payload or {})
        original_payload = payload.copy()

        uid = uid or await self._get_uid(types.Game.STARRAIL)
        payload = dict(role_id=uid, server=utility.recognize_starrail_server(uid), **payload)

        data, params = None, None
        if method == "POST":
            data = payload
        else:
            params = payload

        cache_key: typing.Optional[base.ChronicleCacheKey] = None
        if cache:
            cache_key = base.ChronicleCacheKey(
                types.Game.STARRAIL,
                endpoint,
                uid,
                lang=lang or self.lang,
                params=tuple(original_payload.values()),
            )

        return await self.request_game_record(
            endpoint,
            lang=lang,
            game=types.Game.STARRAIL,
            region=utility.recognize_region(uid, game=types.Game.STARRAIL),
            params=params,
            data=data,
            cache=cache_key,
        )

    async def get_starrail_notes(
        self,
        uid: typing.Optional[int] = None,
        *,
        lang: typing.Optional[str] = None,
        autoauth: bool = True,
    ) -> models.StarRailNote:
        """Get starrail real-time notes."""
        try:
            data = await self._request_starrail_record("note", uid, lang=lang, cache=False)
        except errors.DataNotPublic as e:
            # error raised only when real-time notes are not enabled
            if uid and (await self._get_uid(types.Game.STARRAIL)) != uid:
                raise errors.GenshinException(e.response, "Cannot view real-time notes of other users.") from e
            if not autoauth:
                raise errors.GenshinException(e.response, "Real-time notes are not enabled.") from e

            await self.update_settings(3, True, game=types.Game.STARRAIL)
            data = await self._request_starrail_record("note", uid, lang=lang, cache=False)

        return models.StarRailNote(**data)

    async def get_starrail_user(
        self,
        uid: typing.Optional[int] = None,
        *,
        lang: typing.Optional[str] = None,
    ) -> models.StarRailUserStats:
        """Get starrail user."""
        index_data, basic_info = await asyncio.gather(
            self._request_starrail_record("index", uid, lang=lang),
            self._request_starrail_record("role/basicInfo", uid, lang=lang),
        )
        basic_data = models.StarRailUserInfo(**basic_info)
        return models.StarRailUserStats(**index_data, info=basic_data)

    async def get_starrail_characters(
        self,
        uid: typing.Optional[int] = None,
        *,
        lang: typing.Optional[str] = None,
    ) -> models.StarRailDetailCharacters:
        """Get starrail characters."""
        payload = {"need_wiki": "true"}
        data = await self._request_starrail_record("avatar/info", uid, lang=lang, payload=payload)
        return models.StarRailDetailCharacters(**data)

    async def get_starrail_challenge(
        self,
        uid: typing.Optional[int] = None,
        *,
        previous: bool = False,
        lang: typing.Optional[str] = None,
    ) -> models.StarRailChallenge:
        """Get starrail challenge runs."""
        payload = dict(schedule_type=2 if previous else 1, need_all="true")
        data = await self._request_starrail_record("challenge", uid, lang=lang, payload=payload)
        return models.StarRailChallenge(**data)

    async def get_starrail_rogue(
        self,
        uid: typing.Optional[int] = None,
        *,
        schedule_type: int = 3,
        lang: typing.Optional[str] = None,
    ) -> models.StarRailRogue:
        """Get starrail rogue runs."""
        payload = dict(schedule_type=schedule_type, need_detail="true")
        data = await self._request_starrail_record("rogue", uid, lang=lang, payload=payload)
        return models.StarRailRogue(**data)
