:: update or initialize the subdirectory with proto files
git submodule update --recursive --remote soreco-protocol


:: Generate source files
cd soreco-protocol
.\nanopb-bin\protoc.exe --nanopb_out=../../src/Wifi/Remote/Protocol *.proto