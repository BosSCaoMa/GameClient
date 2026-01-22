#!/usr/bin/env python3
"""
Generate a planner-friendly BattleAttr overview table:
Position × Quality with Lv1 and LvMax comparison.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Dict, Tuple
import pandas as pd

LEVEL_GROWTH_PERCENT = 3
QUALITY_EXPAND_RATES: Tuple[int, ...] = (0, 15, 40, 80, 100, 150, 200, 300)

QUALITY_TYPES: Tuple[Tuple[str, int], ...] = (
    ("WHITE", 1),
    ("GREEN", 2),
    ("BLUE", 3),
    ("PURPLE", 4),
    ("ORANGE", 5),
    ("RED", 6),
    ("GOLD", 7),
)

PRIMARY_FIELDS = ("hp", "atk", "def", "speed")

BASE_TEMPLATE: Dict[str, int] = {
    "hp": 10000,
    "atk": 1000,
    "def": 100,
    "speed": 300,
}

POSITION_OVERRIDES: Dict[str, Dict[str, int]] = {
    "WARRIOR": {"hp": 15000, "atk": 1200, "def": 200, "speed": 350},
    "MAGE": {"hp": 10000, "atk": 1500, "def": 80, "speed": 300},
    "TANK": {"hp": 20000, "atk": 500, "def": 500, "speed": 280},
    "HEALER": {"hp": 12000, "atk": 800, "def": 100, "speed": 320},
    "ASSASSIN": {"hp": 9000, "atk": 1600, "def": 70, "speed": 400},
}


def quality_rate(quality_value: int) -> int:
    return QUALITY_EXPAND_RATES[quality_value]


def apply_quality(value: int, rate: int) -> int:
    return value + (value * rate // 100)


def apply_level(value: int, level: int) -> int:
    if level <= 0:
        return value
    per_level = (value * LEVEL_GROWTH_PERCENT) // 100
    return value + per_level * level


def build_base(position: str) -> Dict[str, int]:
    base = dict(BASE_TEMPLATE)
    base.update(POSITION_OVERRIDES[position])
    return base


def compute_attrs(base: Dict[str, int], rate: int, level: int) -> Dict[str, int]:
    attrs = {}
    for field in PRIMARY_FIELDS:
        v = apply_quality(base[field], rate)
        v = apply_level(v, level)
        attrs[field] = v
    return attrs


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--max-level", type=int, default=100)
    parser.add_argument(
        "--excel",
        type=Path,
        default=Path("battle_attr_planner_overview.xlsx"),
        help="Planner overview Excel output",
    )
    args = parser.parse_args()

    rows = []

    for position in POSITION_OVERRIDES:
        base = build_base(position)
        for quality_name, quality_value in QUALITY_TYPES:
            rate = quality_rate(quality_value)

            lv1 = compute_attrs(base, rate, 1)
            lvmax = compute_attrs(base, rate, args.max_level)

            row = {
                "position": position,
                "quality": quality_name,
            }

            for k in PRIMARY_FIELDS:
                row[f"{k}_Lv1"] = lv1[k]
                row[f"{k}_Lv{args.max_level}"] = lvmax[k]

            rows.append(row)

    df = pd.DataFrame(rows)
    df.to_excel(args.excel, index=False)
    print(f"Planner overview generated -> {args.excel}")


if __name__ == "__main__":
    main()
