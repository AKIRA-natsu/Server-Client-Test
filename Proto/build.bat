@echo off

set PROTOC=bin\protoc.exe
set CPP_OUT=..\Server\Protobuf\
set CSHARP_OUT=..\Client\Assets\Scripts\Protobuf\

rd /S /Q %CPP_OUT%
md %CPP_OUT%

rd /S /Q %CSHARP_OUT%
md %CSHARP_OUT%

%PROTOC%^
  --proto_path=protos^
  --cpp_out=%CPP_OUT%^
  --csharp_out=%CSHARP_OUT%^
  protos/*.proto
