// Variables hole primitive data or reference to data
// Variables are immutable by default
// Rust is a block-scoped language

pub fn run() {
    let name = "Ameya";
    let age = 30;

    // Error: Since variable is immutable
    //age = 38;

    println!("My name is {} and I am {}", name, age);

    let mut newage = 30;

    // Uncomment the following line to remove the compiler warning message.
    // println!("My name is {} and I am {}", name, newage);

    newage = 31;

    println!("My name is {} and I am {}", name, newage);

    // Define constants
    const ID: i32 = 001;
    println!("ID: {}", ID);

    // Assign multiple vars
    let ( my_name, my_age ) = ("Ameya", 30);

    println!("{} is {}", my_name, my_age);
}
