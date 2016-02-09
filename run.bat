:run.bat
@echo off

@echo =========================================================
@echo Demonstrating the functionalities of File Cataloguer
@echo =========================================================

@echo ---------------------------------------------------------
@echo Expected input:
@echo FileCatalogue.exe 'Path' 'File Pattern(s)' '(/s)'
@echo        '(/d)' '(/f)' ('Search Text')
@echo Constraints: /f should be followed by argument 'Search Text'
@echo ---------------------------------------------------------

@echo ---------------------------------------------------------
@echo Demonstrating Requirements 3 and 4
@echo ---------------------------------------------------------
cd .\Debug\

@echo (wihtout recursive search)
@echo Testing the tool with inputs - Path, File Pattern 
FileCatalogue.exe . *.*

@echo (with recursive search)
@echo Testing the tool with inputs - Path, File Pattern, /s 
FileCatalogue.exe . *.cpp /s

@echo ---------------------------------------------------------
@echo Demonstrating Requirement 5
@echo ---------------------------------------------------------


@echo Testing the tool with inputs - Path, File pattern 
@echo (no duplicates displayed)
FileCatalogue.exe .\Test\ *.cpp /s

@echo Testing the tool with inputs - Path, File pattern, /d 
@echo (duplicates displayed this time)
FileCatalogue.exe .\Test\ *.cpp  /d /s


@echo ---------------------------------------------------------
@echo Demonstrating Requirement 6
@echo ---------------------------------------------------------


@echo Testing the tool with inputs - Path, File Pattern, Search Text
FileCatalogue.exe .\Test\ *.txt /f astonished


@echo ---------------------------------------------------------
@echo Demonstrating Requirement 7
@echo ---------------------------------------------------------

@echo Testing the tool with inputs - Path, File Pattern
FileCatalogue.exe . *.*

@echo ---------------------------------------------------------
@echo Demonstrating Requirement 8
@echo ---------------------------------------------------------


@echo Testing the tool with inputs - Path, File Pattern
@echo User to input search string and any specific file patterns in catalogue
FileCatalogue.exe .\Test\ *.cpp



@echo ---------------------------------------------------------
@echo Demonstrating Requirement 9
@echo ---------------------------------------------------------

@echo Testing the tool with inputs - Path, File Pattern
FileCatalogue.exe . *.*

@echo =========================================================
@echo End of Demonstration
@echo =========================================================

cd ..