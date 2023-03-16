@Echo off
title Testing Snake Problem
Echo Testing Snake Problem, Created by Ale.

set /p input="Input da testare (0-6)? "

set foresee=0
set MAX_FORESEE=20
set numMosse=1
set MAX_MOSSE=20
set printMat=n
set bestScore=-100000000
set bestForesee=-1
set bestMosse=-1
:start
    main.exe %input% %foresee% %numMosse% %printMat% > nul 2>&1
    set score=%ERRORLEVEL%
    Echo %foresee% %numMosse% : %score%
    if %score% GTR %bestScore% set bestForesee=%foresee%
    if %score% GTR %bestScore% set bestMosse=%numMosse%
    if %score% GTR %bestScore% set bestScore=%score%
    set /A foresee=foresee+1
    if %foresee% LEQ %MAX_FORESEE% goto start

    set foresee=0
    set /A "numMosse=numMosse+1"
    if %numMosse% LEQ %MAX_MOSSE% goto start


Echo Max Score reached: %bestScore% with params: F=%bestForesee% M=%bestMosse%
