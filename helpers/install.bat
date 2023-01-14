set profile=%1

if "Debug" == "%profile%" (
    set profile=windows_dbg
) else (
    set profile=windows
)

set build_dir=%~dp0..\build
set conan_cmd=conan install .. -pr ..\profiles\%profile% --build=missing

echo Profile is set to: %profile%

echo Build directory: %build_dir%

mkdir %build_dir%
cd %build_dir%

echo %conan_cmd%
%conan_cmd%
