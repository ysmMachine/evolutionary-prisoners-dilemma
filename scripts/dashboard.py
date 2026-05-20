from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd
import streamlit as st


ROOT = Path(__file__).resolve().parents[1]
HISTORY_CSV = ROOT / "results" / "history.csv"

STRATEGY_DESCRIPTIONS = {
    "AlwaysCooperate": "항상 협력합니다. 순수한 협력자의 기준선입니다.",
    "AlwaysDefect": "항상 배신합니다. 단기 이익을 극대화하는 공격적 전략입니다.",
    "TitForTat": "첫 라운드는 협력하고, 이후 상대의 직전 행동을 따라 합니다.",
    "GrimTrigger": "처음에는 협력하지만 상대가 한 번이라도 배신하면 계속 배신합니다.",
    "RandomStrategy": "정해진 확률로 협력 또는 배신을 선택합니다.",
    "Pavlov": "직전 결과가 좋으면 유지하고, 나쁘면 행동을 바꿉니다.",
}


def load_history() -> pd.DataFrame:
    return pd.read_csv(HISTORY_CSV)


def main() -> None:
    st.set_page_config(
        page_title="Evolutionary Prisoner's Dilemma",
        layout="wide",
    )

    st.title("Evolutionary Prisoner's Dilemma Simulator")
    st.caption("책에서 시작한 질문을 C++ 시뮬레이션과 데이터 시각화로 검증합니다.")

    if not HISTORY_CSV.exists():
        st.warning("아직 results/history.csv가 없습니다. 먼저 python scripts/run_simulation.py를 실행하세요.")
        st.stop()

    history = load_history()
    strategy_columns = [column for column in history.columns if column not in {"generation", "avg_score"}]

    st.subheader("세대별 전략 개체 수")
    st.line_chart(history.set_index("generation")[strategy_columns])

    st.subheader("평균 점수 변화")
    st.line_chart(history.set_index("generation")["avg_score"])

    left, right = st.columns([1.1, 1])

    with left:
        st.subheader("최종 세대 전략 분포")
        final_generation = history.iloc[-1]
        final_counts = final_generation[strategy_columns].sort_values(ascending=False)

        fig, ax = plt.subplots(figsize=(8, 4))
        final_counts.plot(kind="bar", ax=ax, color="#2f6f73")
        ax.set_xlabel("Strategy")
        ax.set_ylabel("Count")
        ax.set_title("Final Generation")
        ax.tick_params(axis="x", rotation=30)
        st.pyplot(fig)

    with right:
        st.subheader("보상 행렬")
        payoff = pd.DataFrame(
            {
                "상대 협력": [3, 5],
                "상대 배신": [0, 1],
            },
            index=["나 협력", "나 배신"],
        )
        st.dataframe(payoff, use_container_width=True)

        winner = final_counts.index[0]
        st.metric("최종 우세 전략", winner, int(final_counts.iloc[0]))

    st.subheader("전략 설명")
    columns = st.columns(3)
    for index, (name, description) in enumerate(STRATEGY_DESCRIPTIONS.items()):
        with columns[index % 3]:
            st.markdown(f"**{name}**")
            st.write(description)

    st.subheader("협력은 어떤 조건에서 살아남는가?")
    st.write(
        "반복 게임에서는 한 번의 이익보다 다음 라운드의 관계가 중요해집니다. "
        "TitForTat, GrimTrigger, Pavlov처럼 상대 행동을 기억하고 반응하는 전략은 "
        "무조건 협력하거나 무조건 배신하는 전략 사이에서 균형을 만들 수 있습니다. "
        "돌연변이율, 라운드 수, 보상 행렬을 바꾸며 협력이 살아남는 조건을 직접 비교해보세요."
    )


if __name__ == "__main__":
    main()
