
# Rust Installation


First install `rustup`:

```shell
rustup install stable
rustup default stable
"export PATH=$PATH:~/.cargo/bin" >> ~/.zshrc
```

## Direct

First we need to run:

```sh
rustup target add wasm32-unknown-unknown # add target
cargo install wasm-gc # to compress .wasm files
cargo new --lib rust-wasm
cd rust-wasm
```

Update `Cargo.toml`:

```toml
[package]
name = "rust-wasm"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]
description = "A sample project with wasm-pack"
repository = "https://github.com/myusername/myrepo"
edition = "2021"

[lib]
crate-type = ["cdylib"]

[dependencies]
```

Update `src/lib.rs`:

```rust
#[no_mangle]
pub extern "C" fn add_one(x: i32) -> i32 {
    x + 1
}
```

And build:

```sh
cargo build --target wasm32-unknown-unknown --release # build
ls -la target/wasm32-unknown-unknown/release/rust_wasm.wasm # checkout the file
wasm-gc target/wasm32-unknown-unknown/release/rust_wasm.wasm # compressing 
```

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>title</title>
    <script>
      fetch("target/wasm32-unknown-unknown/release/rust_wasm.wasm")
        .then((response) => response.arrayBuffer())
        .then((bytes) => WebAssembly.instantiate(bytes, {}))
        .then((wasm) => {
          console.log("The answer is: ", wasm.instance.exports.add_one(13));
        });
    </script>
  </head>
  <body>
  </body>
</html>

```


**Notes**

- All communication between the WebAssembly module and host must happen with extern functions. This means that there's only a limited number of types that can be used, mostly primitive types (integers, floats, booleans and pointers). You won't be able to pass complex types unless you're using an additional layer of abstraction on top (which is what `wasm-bindgen` does).
- Large parts of the standard library (including file systems and networking, for instance) is not supported by the `wasm32-unknown-unknown` target. If your WebAssembly host supports WASI (WebAssembly System Interface), you can use the `wasm32-wasi` target instead to compile a module with WASI support, which supports much more of the standard library.

## WASI

Build for WASI
```sh
rustup target add wasm32-wasi # install target
cargo build --target wasm32-wasi --release # build for target
```

Update `index.html`:

```js
      fetch("target/wasm32-unknown-unknown/release/rust_wasm.wasm")
```



## wasm-pack

```sh
cargo new --lib rust-wasm-pack
```

Update `src/lib.rs`:

```rust
use wasm_bindgen::prelude::*;

#[wasm_bindgen]
extern {
    pub fn alert(s: &str);
}

#[wasm_bindgen]
pub fn greet(name: &str) {
    alert(&format!("Hello, {}!", name));
}
```

Update `Cargo.toml`:

```toml
[package]
name = "rust-wasm-pack"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]
description = "A sample project with wasm-pack"
repository = "https://github.com/myusername/myrepo"
edition = "2021"

[lib]
crate-type = ["cdylib"]

[dependencies]
wasm-bindgen = "0.2"
```

And then generate the package

```sh
wasm-pack build --target web
```

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>rust-wasm-pack example</title>
  </head>
  <body>
    <script type="module">
      import init, {greet} from "./pkg/rust_wasm_pack.js";
      init()
        .then(() => {
          greet("WebAssembly")
        });
    </script>
  </body>
</html>
```

Serve the site:
```sh
python3 -m http.server
```

# C

## Clang
First install `clang`.

Create a file called `add.cpp`:

```cpp
#define WASM_EXPORT __attribute__((visibility("default"))) extern "C"
 
WASM_EXPORT int add(int x, int y) {
  return x + y;
}

```

Compile with clang:
```sh
clang++ add.cpp --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o add.wasm
```

And then create a 
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>c-clang wasm example</title>
    <script>
      fetch("add.wasm")
        .then((response) => response.arrayBuffer())
        .then((bytes) => WebAssembly.instantiate(bytes, {}))
        .then((wasm) => {
          console.log("The answer is: ", wasm.instance.exports.add(21, 22));
        });
    </script>
  </head>
  <body></body>
</html>

<!-- Please take a look at -->
<!-- https://depth-first.com/articles/2019/10/16/compiling-c-to-webassembly-and-running-it-without-emscripten/ -->


```

## EMSCRIPTEN

(Install Emscripten.)[https://emscripten.org/docs/getting_started/downloads.html]

Create a `hello.c`:
```c
#include <stdio.h>
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE void hello(){
    printf("Hello Woraald\n");
}

int main() {
    hello();
}
```

Compile!
```sh
emcc hello.c -s WASM=1 -o index.html --shell-file html_template/shell_minimal.html
```
