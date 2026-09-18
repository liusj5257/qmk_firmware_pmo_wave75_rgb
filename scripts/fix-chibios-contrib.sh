#!/usr/bin/env bash
# 修复 lib/chibios-contrib 里 FS026 的 include guard 宏名写错的问题。
#
# 上游 ChibiOS-Contrib 的
#   os/common/ext/CMSIS/ES32/FS026/system_fs026.h
# 中写的是：
#   #ifndef __SYSTEM_FS026_H__
#   #define __SYSTEM_ES32F0283_H__   <-- 宏名不一致，重复包含时会报错
# 必须改成 __SYSTEM_FS026_H__，否则 hm/wave75_rgb 无法编译。
#
# 用法（在仓库根目录执行）：
#   ./scripts/fix-chibios-contrib.sh
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TARGET="$REPO_ROOT/lib/chibios-contrib/os/common/ext/CMSIS/ES32/FS026/system_fs026.h"

if [[ ! -f "$TARGET" ]]; then
    echo "[ERROR] 找不到 $TARGET"
    echo "        请先拉取子模块：git submodule update --init --recursive"
    exit 1
fi

if grep -q '^#define __SYSTEM_FS026_H__' "$TARGET"; then
    echo "[OK] system_fs026.h 已是修复后的状态，无需修改"
    exit 0
fi

if ! grep -q '^#define __SYSTEM_ES32F0283_H__' "$TARGET"; then
    echo "[ERROR] 未找到预期的宏定义，请手动检查：$TARGET"
    exit 1
fi

sed -i 's/^#define __SYSTEM_ES32F0283_H__/#define __SYSTEM_FS026_H__/' "$TARGET"
echo "[OK] 已修复：$TARGET"
