clang++ add.cpp --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o add.wasm