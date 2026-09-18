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
#   powershell -ExecutionPolicy Bypass -File .\scripts\fix-chibios-contrib.ps1
$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$target   = Join-Path $repoRoot 'lib/chibios-contrib/os/common/ext/CMSIS/ES32/FS026/system_fs026.h'

if (-not (Test-Path -LiteralPath $target)) {
    Write-Host "[ERROR] 找不到 $target"
    Write-Host "        请先拉取子模块：git submodule update --init --recursive"
    exit 1
}

$content = Get-Content -LiteralPath $target -Raw

if ($content -match '(?m)^#define __SYSTEM_FS026_H__') {
    Write-Host '[OK] system_fs026.h 已是修复后的状态，无需修改'
    exit 0
}

if ($content -notmatch '(?m)^#define __SYSTEM_ES32F0283_H__') {
    Write-Host "[ERROR] 未找到预期的宏定义，请手动检查：$target"
    exit 1
}

$content = $content -replace '(?m)^#define __SYSTEM_ES32F0283_H__', '#define __SYSTEM_FS026_H__'
Set-Content -LiteralPath $target -Value $content -NoNewline
Write-Host "[OK] 已修复：$target"
