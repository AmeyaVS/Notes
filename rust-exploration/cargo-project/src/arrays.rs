// Arrays - Fixed list where elements are the same data types

use std::mem;

pub fn run() {
    //let numbers: [i32; 5] = [1, 2, 3, 4, 5];
    let mut numbers: [i32; 5] = [1, 2, 3, 4, 5];
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

    println!("{:?}", numbers);

    // Get array length
    println!("Array Length: {}", numbers.len());

    // Arrays are stack alocated
    //println!("Array occupies {} bytes", std::mem::size_of_val(&numbers));
    println!("Array occupies {} bytes", mem::size_of_val(&numbers));

    // Get Slice
    //let slice: &[i32] = &numbers;
    let slice: &[i32] = &numbers[1..3];

    println!("Slice: {:?}", slice);
}
