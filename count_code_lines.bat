@echo off
setlocal EnableDelayedExpansion
set /a total=0
(
 for %%f in (*.cpp) do (
  for /f %%a in ('type "%%f"^|find /C /v  "" ') do set /a total+=%%a&echo %%f %%a
 )
 for %%f in (*.h) do (
  for /f %%a in ('type "%%f"^|find /C /v  "" ') do set /a total+=%%a&echo %%f %%a
 )
 echo total !total!
)
pause

