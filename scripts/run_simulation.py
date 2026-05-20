from __future__ import annotations

import json
import os
import platform
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BUILD_DIR = Path("build")
CONFIG_PATH = ROOT / "config" / "default_config.json"


def load_config() -> dict:
    default_config = {
        "population_size": 100,
        "generations": 100,
        "rounds_per_match": 100,
        "mutation_rate": 0.02,
        "seed": 42,
        "output_csv": "results/history.csv",
    }

    if not CONFIG_PATH.exists():
        return default_config

    with CONFIG_PATH.open("r", encoding="utf-8") as file:
        user_config = json.load(file)

    return {**default_config, **user_config}


def executable_name() -> str:
    name = "evo_pd.exe" if platform.system() == "Windows" else "evo_pd"
    return name


def configure_toolchain_environment() -> dict[str, str]:
    return os.environ.copy()


def find_tool(tool_name: str, env: dict[str, str]) -> str | None:
    path_value = env.get("Path", env.get("PATH"))
    found = shutil.which(tool_name, path=path_value)
    if found:
        return tool_name

    return None


def cmake_configure_command(env: dict[str, str]) -> list[str]:
    cmake = find_tool("cmake", env) or "cmake"
    command = [cmake, "-S", ".", "-B", str(BUILD_DIR)]

    path_value = env.get("Path", env.get("PATH"))
    ninja = shutil.which("ninja", path=path_value)
    clang = find_tool("clang++", env)

    if ninja and clang:
        command.extend(["-G", "Ninja", f"-DCMAKE_CXX_COMPILER={clang}"])

    return command


def find_executable() -> Path:
    name = executable_name()
    candidates = [
        BUILD_DIR / name,
        BUILD_DIR / "Debug" / name,
        BUILD_DIR / "Release" / name,
        BUILD_DIR / "RelWithDebInfo" / name,
    ]

    for candidate in candidates:
        if (ROOT / candidate).exists():
            return candidate

    matches = list((ROOT / BUILD_DIR).rglob(name))
    if matches:
        return matches[0].relative_to(ROOT)

    raise FileNotFoundError(f"C++ 실행 파일을 찾을 수 없습니다: {name}")


def run_command(command: list[str], cwd: Path, env: dict[str, str]) -> None:
    print("$", " ".join(command))
    subprocess.run(command, cwd=cwd, check=True, env=env)


def main() -> None:
    config = load_config()
    env = configure_toolchain_environment()

    run_command(cmake_configure_command(env), ROOT, env)
    cmake = find_tool("cmake", env) or "cmake"
    run_command([cmake, "--build", str(BUILD_DIR)], ROOT, env)

    output_csv = Path(config["output_csv"])
    (ROOT / output_csv).parent.mkdir(parents=True, exist_ok=True)

    command = [
        str(find_executable()),
        str(config["population_size"]),
        str(config["generations"]),
        str(config["rounds_per_match"]),
        str(config["mutation_rate"]),
        str(config["seed"]),
        str(output_csv),
    ]
    run_command(command, ROOT, env)

    print(f"\nSimulation finished: {output_csv}")
    print("Dashboard: streamlit run scripts/dashboard.py")


if __name__ == "__main__":
    main()
