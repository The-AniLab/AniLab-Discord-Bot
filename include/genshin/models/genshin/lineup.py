"""Genshin lineup models."""
from __future__ import annotations

import datetime
import typing

import pydantic

from genshin.models.genshin import character
from genshin.models.model import Aliased, APIModel, Unique

__all__ = [
    "Lineup",
    "LineupAbyssScenarios",
    "LineupAbyssScenarios",
    "LineupArtifactStat",
    "LineupArtifactStatFields",
    "LineupCharacter",
    "LineupCharacterPreview",
    "LineupFields",
    "LineupFields",
    "LineupPreview",
    "LineupPrimaryArtifactStat",
    "LineupScenario",
    "LineupWorldScenarios",
    "PartialLineupArtifactSet",
    "PartialLineupCharacter",
    "PartialLineupWeapon",
]


class PartialLineupCharacter(character.BaseCharacter):
    """Partial lineup character."""

    element: str
    rarity: int = Aliased("level")
    weapon_type: str = Aliased("weapon_cat_id")
    icon: str = Aliased("head_icon")
    pc_icon: str = Aliased("standard_icon")

    def __init__(self, _frame: int = 1, **data: typing.Any) -> None:
        # rename "icon" due to pydantic's aliasing
        icons_amount = sum(1 for key, value in data.items() if "icon" in key if value)
        if data.get("pc_icon") and data.get("icon"):
            ...
        elif icons_amount >= 2 and "static" in data.get("icon", ""):
            data["standard_icon"] = data.pop("icon")

        super().__init__(_frame=_frame + 3, **data)  # type: ignore

    @pydantic.validator("element", pre=True)
    def __parse_element(cls, value: typing.Any) -> str:
        if isinstance(value, str) and not value.isdigit():
            return value

        return {
            1: "Pyro",
            2: "Anemo",
            3: "Geo",
            4: "Dendro",
            5: "Electro",
            6: "Hydro",
            7: "Cryo",
        }[int(value)]

    @pydantic.validator("weapon_type", pre=True)
    def __parse_weapon_type(cls, value: typing.Any) -> str:
        if isinstance(value, str) and not value.isdigit():
            return value

        return {
            0: "Unknown",
            1: "Sword",
            10: "Catalyst",
            11: "Claymore",
            12: "Bow",
            13: "Polearm",
        }[int(value)]


class PartialLineupWeapon(APIModel, Unique):
    """Partial lineup weapon."""

    id: int
    name: str
    icon: str
    rarity: int = Aliased("level")
    type: str = Aliased("cat_id")

    @pydantic.validator("type", pre=True)
    def __parse_weapon_type(cls, value: int) -> str:
        if isinstance(value, str) and not value.isdigit():
            return value

        return {
            1: "Sword",
            10: "Catalyst",
            11: "Claymore",
            12: "Bow",
            13: "Polearm",
        }[value]


class PartialLineupArtifactSet(APIModel, Unique):
    """Parital lineup artifact set."""

    id: int
    name: str
    icon: str
    rarity: int = Aliased("level")


class LineupArtifactStatFields(APIModel):
    """Lineup artifact stat fields."""

    flower: typing.Mapping[int, str] = pydantic.Field(artifact_id=1)
    plume: typing.Mapping[int, str] = pydantic.Field(artifact_id=2)
    sands: typing.Mapping[int, str] = pydantic.Field(artifact_id=3)
    goblet: typing.Mapping[int, str] = pydantic.Field(artifact_id=4)
    circlet: typing.Mapping[int, str] = pydantic.Field(artifact_id=5)

    secondary_stats: typing.Mapping[int, str] = Aliased("reliquary_sec_attr")

    @pydantic.root_validator(pre=True)
    def __flatten_stats(cls, values: typing.Dict[str, typing.Any]) -> typing.Dict[str, typing.Any]:
        """Name certain stats."""
        if "reliquary_fst_attr" not in values:
            return values

        artifact_ids = {
            field.field_info.extra["artifact_id"]: name
            for name, field in cls.__fields__.items()
            if field.field_info.extra.get("artifact_id")
        }

        for scenario in values["reliquary_fst_attr"]:
            if scenario["key"] not in artifact_ids:
                continue

            name = artifact_ids[scenario["key"]]
            values[name] = scenario["value"]

        return values

    @pydantic.validator("secondary_stats", "flower", "plume", "sands", "goblet", "circlet", pre=True)
    def __parse_secondary_stats(cls, value: typing.Any) -> typing.Dict[int, str]:
        if not isinstance(value, typing.Sequence):
            return value

        return {stat["id"]: stat["name"] for stat in value}  # type: ignore

    @property
    def all_stats(self) -> typing.Mapping[int, str]:
        """All possible stats for any artifact."""
        return {**self.flower, **self.plume, **self.sands, **self.goblet, **self.circlet, **self.secondary_stats}


class LineupFields(APIModel):
    """Configuration lineup fields."""

    characters: typing.Sequence[PartialLineupCharacter] = Aliased("all_avatar")
    weapons: typing.Sequence[PartialLineupWeapon] = Aliased("all_weapon")
    artifacts: typing.Sequence[PartialLineupArtifactSet] = Aliased("all_set")

    artifact_stats: LineupArtifactStatFields = Aliased("extra_config")


class LineupArtifactStat(APIModel, Unique):
    """Lineup artifact stat."""

    id: int
    name: str


class LineupPrimaryArtifactStat(LineupArtifactStat):
    """Lineup primary artifact stat."""

    artifact_type: int = Aliased("cat_id")


class LineupScenario(APIModel, Unique):
    """Lineup scenario such as domain, boss or spiral abyss."""

    id: int
    name: str
    children: typing.Sequence[LineupScenario]

    @pydantic.root_validator(pre=True)
    def __flatten_scenarios(cls, values: typing.Dict[str, typing.Any]) -> typing.Dict[str, typing.Any]:
        """Name certain scenarios."""
        scenario_ids = {
            field.field_info.extra["scenario_id"]: name
            for name, field in cls.__fields__.items()
            if field.field_info.extra.get("scenario_id")
        }

        for scenario in values["children"]:
            if scenario["id"] not in scenario_ids:
                continue

            name = scenario_ids[scenario["id"]]
            values[name] = scenario

        return values

    @property
    def all_children(self) -> typing.Sequence[LineupScenario]:
        """Get all children of this scenario."""
        children = list(self.children)
        for child in self.children:
            children.extend(child.all_children)

        return children


class LineupWorldScenarios(LineupScenario):
    """Lineup world scenario."""

    trounce_domains: LineupScenario = pydantic.Field(scenario_id=3)
    domain_challenges: LineupScenario = pydantic.Field(scenario_id=9)
    battles: LineupScenario = pydantic.Field(scenario_id=24)


class LineupAbyssScenarios(LineupScenario):
    """Lineup abyss scenario."""

    corridor: LineupScenario = pydantic.Field(scenario_id=42)
    spire: LineupScenario = pydantic.Field(scenario_id=41)


class LineupScenarios(LineupScenario):
    """Lineup scenarios."""

    world: LineupWorldScenarios = pydantic.Field(scenario_id=1)
    abyss: LineupAbyssScenarios = pydantic.Field(scenario_id=2)


class LineupCharacterPreview(PartialLineupCharacter):
    """Partial lineup character preview."""

    role: str = Aliased("avatar_tag")

    icon: str = Aliased("standard_icon")
    pc_icon: str = Aliased("pc_icon")

    @pydantic.validator("role", pre=True)
    def __parse_role(cls, value: typing.Any) -> str:
        if isinstance(value, str):
            return value

        return value["name"]


class LineupCharacter(LineupCharacterPreview):
    """Lineup character."""

    icon: str = Aliased("head_icon")
    pc_icon: str = Aliased("standard_icon")

    weapon: PartialLineupWeapon
    artifacts: typing.Sequence[PartialLineupArtifactSet] = Aliased("set_list")

    artifact_attributes: typing.Sequence[LineupPrimaryArtifactStat] = Aliased("first_attr")
    secondary_attributes: typing.Sequence[LineupArtifactStat] = Aliased("secondary_attr_name")


class LineupPreview(APIModel, Unique):
    """Lineup preview."""

    id: str  # type: ignore
    title: str
    scenario_ids: typing.Sequence[int] = Aliased("tag_ids")

    characters: typing.Sequence[typing.Sequence[LineupCharacter]] = Aliased("avatar_group")

    author_id: int = Aliased("account_uid")
    author_nickname: str = Aliased("nickname")
    author_icon: str = Aliased("avatar_url")
    author_level: int = Aliased("level")

    likes: int = Aliased("like_cnt")
    comments: int = Aliased("comment_cnt")
    created_at: datetime.datetime

    original_lang: str = Aliased("trans_from")

    @pydantic.validator("characters", pre=True)
    def __parse_characters(cls, value: typing.Any) -> typing.Any:
        if isinstance(value[0], typing.Sequence):
            return value

        return [[character for character in group["group"]] for group in value]


class Lineup(LineupPreview):
    """Lineup."""

    description: str
    views: int = Aliased("view_cnt")

    characters: typing.Sequence[typing.Sequence[LineupCharacter]] = Aliased("avatar_group")
