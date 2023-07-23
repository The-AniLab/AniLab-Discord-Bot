"""Genshin chronicle notes."""
import datetime
import typing

import pydantic

from genshin.models.genshin import character
from genshin.models.model import Aliased, APIModel

__all__ = ["Expedition", "ExpeditionCharacter", "Notes"]


def _process_timedelta(time: typing.Union[int, datetime.timedelta, datetime.datetime]) -> datetime.datetime:
    if isinstance(time, int):
        time = datetime.datetime.fromtimestamp(time).astimezone()

    if isinstance(time, datetime.timedelta):
        time = datetime.datetime.now().astimezone() + time

    if time < datetime.datetime(2000, 1, 1).astimezone():
        delta = datetime.timedelta(seconds=int(time.timestamp()))
        time = datetime.datetime.now().astimezone() + delta

    time = time.replace(second=0, microsecond=0)

    return time


class ExpeditionCharacter(character.BaseCharacter):
    """Expedition character."""


class Expedition(APIModel):
    """Real-Time note expedition."""

    character: ExpeditionCharacter = Aliased("avatar_side_icon")
    status: typing.Literal["Ongoing", "Finished"]
    remaining_time: datetime.timedelta = Aliased("remained_time")

    @property
    def finished(self) -> bool:
        """Whether the expedition has finished."""
        return self.remaining_time <= datetime.timedelta(0)

    @property
    def completion_time(self) -> datetime.datetime:
        return datetime.datetime.now().astimezone() + self.remaining_time

    @pydantic.validator("character", pre=True)
    def __complete_character(cls, v: typing.Any) -> typing.Any:
        if isinstance(v, str):
            return dict(icon=v)  # type: ignore

        return v


class TransformerTimedelta(datetime.timedelta):
    """Transformer recovery time."""

    @property
    def timedata(self) -> typing.Tuple[int, int, int, int]:
        seconds: int = super().seconds
        days: int = super().days
        hour, second = divmod(seconds, 3600)
        minute, second = divmod(second, 60)

        return days, hour, minute, second

    @property
    def hours(self) -> int:
        return self.timedata[1]

    @property
    def minutes(self) -> int:
        return self.timedata[2]

    @property
    def seconds(self) -> int:
        return self.timedata[3]


class Notes(APIModel):
    """Real-Time notes."""

    current_resin: int
    max_resin: int
    remaining_resin_recovery_time: datetime.timedelta = Aliased("resin_recovery_time")

    current_realm_currency: int = Aliased("current_home_coin")
    max_realm_currency: int = Aliased("max_home_coin")
    remaining_realm_currency_recovery_time: datetime.timedelta = Aliased("home_coin_recovery_time")

    completed_commissions: int = Aliased("finished_task_num")
    max_commissions: int = Aliased("total_task_num")
    claimed_commission_reward: bool = Aliased("is_extra_task_reward_received")

    remaining_resin_discounts: int = Aliased("remain_resin_discount_num")
    max_resin_discounts: int = Aliased("resin_discount_num_limit")

    remaining_transformer_recovery_time: typing.Optional[TransformerTimedelta]

    expeditions: typing.Sequence[Expedition]
    max_expeditions: int = Aliased("max_expedition_num")

    @property
    def resin_recovery_time(self) -> datetime.datetime:
        """The time when resin will be recovered."""
        return datetime.datetime.now().astimezone() + self.remaining_resin_recovery_time

    @property
    def realm_currency_recovery_time(self) -> datetime.datetime:
        """The time when realm currency will be recovered."""
        return datetime.datetime.now().astimezone() + self.remaining_realm_currency_recovery_time

    @property
    def transformer_recovery_time(self) -> typing.Optional[datetime.datetime]:
        """The time the transformer will be recovered."""
        if self.remaining_transformer_recovery_time is None:
            return None

        remaining = datetime.datetime.now().astimezone() + self.remaining_transformer_recovery_time
        return remaining

    @pydantic.root_validator(pre=True)
    def __flatten_transformer(cls, values: typing.Dict[str, typing.Any]) -> typing.Dict[str, typing.Any]:
        if "transformer_recovery_time" in values:
            return values

        if values.get("transformer") and values["transformer"]["obtained"]:
            t = values["transformer"]["recovery_time"]
            delta = TransformerTimedelta(days=t["Day"], hours=t["Hour"], minutes=t["Minute"], seconds=t["Second"])
            values["remaining_transformer_recovery_time"] = delta
        else:
            values["remaining_transformer_recovery_time"] = None

        return values
