$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot = Split-Path -Parent (Split-Path -Parent $ScriptDir)

# Easy-to-edit executable paths.
$PSeIntExe = if ($env:PSEINT_EXE) { $env:PSEINT_EXE } elseif (Test-Path (Join-Path $RepoRoot "bin/pseint_cbot.exe")) { Join-Path $RepoRoot "bin/pseint_cbot.exe" } else { Join-Path $RepoRoot "bin/pseint.exe" }
$PSExportExe = if ($env:PSEXPORT_EXE) { $env:PSEXPORT_EXE } else { Join-Path $RepoRoot "bin/psexport.exe" }

$TestsDir = $ScriptDir
$ExpectedDir = Join-Path $TestsDir "expected"
$OutDir = Join-Path $TestsDir "out"

$TestNames = @(
    "01_variables_asignacion",
    "02_escribir",
    "03_dimension_1d",
    "04_paracada_1d",
    "05_paracada_2d_no_soportado",
    "06_acceso_2d",
    "07_dimension_2d_escribir",
    "08_redimension_1d_no_soportado",
    "09_redimension_2d_no_soportado",
    "10_subindice_expresion"
)

function Normalize-Lines {
    param(
        [string]$Path
    )

    $lines = Get-Content -Path $Path
    $normalized = New-Object System.Collections.Generic.List[string]

    foreach ($line in $lines) {
        if ($line -like "// Este codigo ha sido generado*") { continue }
        if ($line -like "// Es posible que el codigo generado*") { continue }
        if ($line -like "// errores por favor reportelos*") { continue }
        if ($line -like "// Approximate expected output*") { continue }
        if ($line -like "// Exportador CBOT V*") { continue }

        $trimmedRight = $line.TrimEnd()
        if ([string]::IsNullOrWhiteSpace($trimmedRight)) { continue }

        $compact = Normalize-CodeLine -Line $trimmedRight
        $normalized.Add($compact)
    }

    return $normalized
}

function Normalize-CodeSegment {
    param(
        [string]$Segment
    )

    $s = [regex]::Replace($Segment, "\s+", " ")
    $s = [regex]::Replace($s, "\s*(<=|>=|==|!=|&&|\|\|)\s*", '$1')
    $s = [regex]::Replace($s, "\s*([,;()\[\]{}])\s*", '$1')
    $s = [regex]::Replace($s, "\s*([+\-*/%<>])\s*", '$1')
    $s = [regex]::Replace($s, "\s*=\s*", '=')
    return $s.Trim()
}

function Normalize-CodeLine {
    param(
        [string]$Line
    )

    $pattern = '"(?:\\.|[^"\\])*"'
    $matches = [regex]::Matches($Line, $pattern)
    if ($matches.Count -eq 0) {
        return Normalize-CodeSegment -Segment $Line
    }

    $parts = New-Object System.Collections.Generic.List[string]
    $pos = 0

    foreach ($match in $matches) {
        if ($match.Index -gt $pos) {
            $parts.Add((Normalize-CodeSegment -Segment $Line.Substring($pos, $match.Index - $pos)))
        }
        $parts.Add($match.Value)
        $pos = $match.Index + $match.Length
    }

    if ($pos -lt $Line.Length) {
        $parts.Add((Normalize-CodeSegment -Segment $Line.Substring($pos)))
    }

    return (($parts -join '')).Trim()
}

function Compare-TestOutput {
    param(
        [string]$ExpectedPath,
        [string]$ActualPath
    )

    $expected = Normalize-Lines -Path $ExpectedPath
    $actual = Normalize-Lines -Path $ActualPath

    $diff = Compare-Object -ReferenceObject $expected -DifferenceObject $actual
    return $diff
}

function Get-PresentationHint {
    param(
        [object[]]$Diff
    )

    if (-not $Diff -or $Diff.Count -eq 0) {
        return $null
    }

    $nonBanner = @(
        $Diff | Where-Object {
            $_.InputObject -notlike "*Exportador CBOT V2*" -and
            $_.InputObject -notlike "*Approximate expected output*"
        }
    )

    if ($nonBanner.Count -eq 0) {
        return "diferencia de presentacion probable"
    }

    return $null
}

if (-not (Test-Path $PSeIntExe)) {
    Write-Host "ERROR: executable not found: $PSeIntExe" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $PSExportExe)) {
    Write-Host "ERROR: executable not found: $PSExportExe" -ForegroundColor Red
    exit 1
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$results = New-Object System.Collections.Generic.List[object]

foreach ($name in $TestNames) {
    $src = Join-Path $TestsDir ($name + ".pseint")
    $psd = Join-Path $OutDir ($name + ".psd")
    $actual = Join-Path $OutDir ($name + ".cbot")
    $expected = Join-Path $ExpectedDir ($name + ".cbot")

    Remove-Item -Force -ErrorAction SilentlyContinue $psd, $actual

    $status = "OK"
    $note = ""
    $diffPreview = @()

    try {
        if (-not (Test-Path $src)) {
            throw "missing source file"
        }
        if (-not (Test-Path $expected)) {
            throw "missing expected file"
        }

        & $PSeIntExe $src "--draw" $psd "--norun" "--nouser" | Out-Null
        if (-not (Test-Path $psd)) {
            throw "PSD generation failed"
        }

        & $PSExportExe "--lang=cbot" $psd $actual | Out-Null
        if (-not (Test-Path $actual)) {
            throw "CBOT export failed"
        }

        $diff = Compare-TestOutput -ExpectedPath $expected -ActualPath $actual
        if ($diff) {
            $status = "DIFF"
            $hint = Get-PresentationHint -Diff $diff
            if ($hint) {
                $note = $hint
            }
            $diffPreview = @($diff | Select-Object -First 6)
        }
    }
    catch {
        $status = "ERROR"
        $note = $_.Exception.Message
    }

    $results.Add([pscustomobject]@{
        Name = $name
        Status = $status
        Note = $note
        DiffPreview = $diffPreview
    }) | Out-Null
}

Write-Host ""
Write-Host "CBOT test run" -ForegroundColor Cyan
Write-Host "Repo: $RepoRoot"
Write-Host "Out : $OutDir"
Write-Host ""

foreach ($result in $results) {
    $color = "Green"
    if ($result.Status -eq "DIFF") { $color = "Yellow" }
    if ($result.Status -eq "ERROR") { $color = "Red" }

    $line = "{0} - {1}" -f $result.Name, $result.Status
    if ($result.Note) {
        $line += " ({0})" -f $result.Note
    }
    Write-Host $line -ForegroundColor $color

    if ($result.DiffPreview.Count -gt 0) {
        foreach ($item in $result.DiffPreview) {
            Write-Host ("  {0} {1}" -f $item.SideIndicator, $item.InputObject)
        }
    }
}

$okCount = @($results | Where-Object { $_.Status -eq "OK" }).Count
$diffCount = @($results | Where-Object { $_.Status -eq "DIFF" }).Count
$errCount = @($results | Where-Object { $_.Status -eq "ERROR" }).Count

Write-Host ""
Write-Host ("Summary: OK={0} DIFF={1} ERROR={2}" -f $okCount, $diffCount, $errCount) -ForegroundColor Cyan

if ($errCount -gt 0) {
    exit 1
}

if ($diffCount -gt 0) {
    exit 2
}

exit 0
