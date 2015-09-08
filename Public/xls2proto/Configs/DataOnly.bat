@echo off

set XLS_NAME=%1
set SHEET_NAME=%2
set DATA_DEST=%3



echo.
echo =========Compilation of %XLS_NAME%.xls=========


::---------------------------------------------------
::��һ������xls����xls_deploy_toolת��data
::---------------------------------------------------
set STEP1_XLS2PROTO_PATH=step1_xls2proto

@echo on
cd %STEP1_XLS2PROTO_PATH%

@echo off
echo TRY TO DELETE TEMP FILES:
del *_pb2.py
del *_pb2.pyc
del *.proto
del *.data
del *.log
del *.txt

@echo on
python xls_deploy_tool.py %SHEET_NAME% ..\..\Assets\DataConfig\%XLS_NAME%.xlsx 0


cd ..

::---------------------------------------------------
::����������data��cs����Assets��
::---------------------------------------------------

@echo off
set OUT_PATH=..\Assets
set DATA_DEST=StreamingAssets\DataConfig
set CS_DEST=Scripts\ProtoGen


@echo on
copy %STEP1_XLS2PROTO_PATH%\*.data %OUT_PATH%\%DATA_DEST%

::---------------------------------------------------
::���Ĳ�������м��ļ�
::---------------------------------------------------
@echo off
echo TRY TO DELETE TEMP FILES:
cd %STEP1_XLS2PROTO_PATH%
del *_pb2.py
del *_pb2.pyc
del *.proto
del *.data
del *.log
del *.txt
cd ..
cd %STEP2_PROTO2CS_PATH%
del *.cs
del *.protodesc
del *.txt
cd ..

::---------------------------------------------------
::���岽������
::---------------------------------------------------
cd ..

pause

@echo on