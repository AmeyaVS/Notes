// Vectors - Resizable arrays

use std::mem;

pub fn run() {
    //let numbers: [i32; 5] = [1, 2, 3, 4, 5];
    let mut numbers: Vec<i32> = vec![1, 2, 3, 4, 5];
    // Error:
    //let numbers: [i32; 5] = [1, 2, 3, 4];
    // Error:
    //let numbers: [i32; 5] = [1, 2, 3, 4, "Hello"];

    println!("{:?}", numbers);

    // Get single val
    println!("Single value: {}", numbers[0]);

    // Re-assign value
    // This requires the array declaration to be mutable
    numbers[2] = 20;

    // Add on to vector
    numbers.push(6);
    numbers.push(7);

    // Pop off last value
    numbers.pop();

    println!("{:?}", numbers);

    // Get array length
    println!("Vector Length: {}", numbers.len());

    // Vectors are stack alocated
    //println!("Vector occupies {} bytes", std::mem::size_of_val(&numbers));
    println!("Vector occupies {} bytes", mem::size_of_val(&numbers));

    // Get Slice
    //let slice: &[i32] = &numbers;
    let slice: &[i32] = &numbers[1..3];

    println!("Slice: {:?}", slice);

    // Loop through vector values
    for x in numbers.iter() {
        println!("Number: {}", x);
    }

    // Loop & mutate values
    for x in numbers.iter_mut() {
        *x *= 2;
    }

    println!("Numbers Vec: {:?}", numbers);
}
