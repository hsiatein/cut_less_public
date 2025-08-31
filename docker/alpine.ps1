$SCRIPT_DIR = Split-Path -Parent (Resolve-Path $MyInvocation.MyCommand.Path)
Set-Location $SCRIPT_DIR
Write-Output (Get-Location)

if (-not (docker image inspect alpine_base:latest -ErrorAction SilentlyContinue)) {
    Write-Output "构建 alpine_base"
    docker build --network host --progress=plain -f alpine_base.dockerfile -t alpine_base .
}

New-Item -ItemType Directory -Force -Path "sources" | Out-Null
New-Item -ItemType Directory -Force -Path "sources/external" | Out-Null

Copy-Item "./Cmago.toml" "sources/"
Copy-Item "../assets/config.json" "sources/"
Copy-Item "../cl_base" "sources/" -Recurse
Copy-Item "../cl_solver" "sources/" -Recurse
Copy-Item "../executables" "sources/" -Recurse
Copy-Item "../web_api" "sources/" -Recurse
Copy-Item "../external/HiGHS" "sources/external" -Recurse
Copy-Item "../external/argparse" "sources/external" -Recurse
Copy-Item "../external/json" "sources/external" -Recurse
Copy-Item "../external/googletest" "sources/external" -Recurse

docker build --network host --progress=plain -f alpine.dockerfile -t cut_less_alpine .

Remove-Item -Recurse -Force "./sources"
