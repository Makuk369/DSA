import subprocess
import time

def run_exe_with_io(exe_path, input_txt, output_txt):
    # Open the input file to read values
    with open(input_txt, 'r') as input_file:
        input_data = input_file.read()


    
    # Open the output file to store the output
    with open(output_txt, 'w') as output_file:
        # Run the executable
        process = subprocess.Popen(
            exe_path, 
            stdin=subprocess.PIPE, 
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Pass input to the executable and capture output
        stdout, stderr = process.communicate(input=input_data)

        
        
        # Write output to file
        output_file.write(stdout)
        
        # If there's any error output, also log it
        if stderr:
            output_file.write("\nERROR OUTPUT:\n" + stderr)
    
    # print(f"Output saved to {output_txt}")

# Example usage
exe_path = "RBTree.exe"
input_txt = "Testing\\input.txt"
output_txt = "Testing\\output.txt"

testRepeats = int(input("Test repeats: "))
funcRepeats = int(input("Function repeats: "))

for _ in range(testRepeats):
    # Start the timer
    start_time = time.perf_counter()

    for _ in range(funcRepeats):
        run_exe_with_io(exe_path, input_txt, output_txt)

    # Stop the timer
    end_time = time.perf_counter()
    execution_time = end_time - start_time
    # print(f"Start = {start_time}, end = {end_time}")

    print(f"Execution completed in {execution_time} seconds.")