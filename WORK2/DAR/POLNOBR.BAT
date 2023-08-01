@ECHO OFF
rem call c:\TC\WORK2\DAR\kanali.bat %1 %2 
COPY c:\TC\WORK2\DAR\DATA\%1.dat c:\TC\WORK2\DAR\ps.dat 
rem call c:\TC\WORK2\DAR\furie.bat    
c:\TC\WORK2\DAR\file_fft.exe /c1 /v1 /n2048 /d12
c:\TC\WORK2\confur2.exe %1 %2 
