pub fn run() {
    // Print to console
    println!("Hello from print.rs file!");

    println!("Number: {}", 1);

    // Basic Formatting
    println!("{} is from {}", "Brad", "Mass");

    // Positional Arguments
    println!("{0} is from {1} and {0} likes to {2}", "Brad", "Mass", "code");

    // Named Arguments
    println!("{name} likes to {activity}",
             name = "John",
             activity = "Baseball!"
             );

    // Plaeholder tratis
    println!("Binary: {:b} Hex: {:x} Octal: {:o}", 10, 10, 10);
}
