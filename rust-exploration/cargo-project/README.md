# A sample project created using Cargo

Steps to create a Rust project using Cargo

1. Create the base project:

   ```sh
   # Creating the project directory manually
   $ mkdir cargo-project
   $ cd cargo-project
   $ cargo init


   # Cargo creating the directory iteself
   $ cargo new cargo-project
   ```
2. Observe Following Directory Structure:

   ```sh
   $ ls -al

   cargo-project
   |- Cargo.toml //< Project Metadata similar to a Python project pip configurations.
   |- src
   |  |- main.rs //< Entry point for the application
   ```
3. Building and running the Project

   ```sh
   # This command builds and executes the binary
   $ cargo run

   # This command just builds the executable
   $ cargo build

   # This command build the executable in release/production mode.
   $ cargo build --release

   # Manualy running the binary
   $ ./target/debug/cargo-project
   ```
