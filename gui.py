import tkinter as tk
from tkinter import filedialog
import subprocess

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("RISC-V Emulator")

        self.file_path = tk.StringVar()
        self.input_text = tk.StringVar()
        self.output_text = tk.StringVar()

        self.create_widgets()

    def create_widgets(self):
        # Main Title
        self.main_title = tk.Label(self.root, text="RISC-V Emulator", font=('Helvetica', 20, 'bold'))
        self.main_title.grid(row=0, column=0, columnspan=3, padx=10, pady=10, sticky="nsew")

        # Instructions Label
        instructions_text = ("Instructions on how to use the program:\n\n"
                             "1. Upload your file.\n"
                             "2. Enter any required input.\n"
                             "3. Click 'Execute Program' to run.")
        self.instructions_label = tk.Label(self.root, text=instructions_text, justify=tk.LEFT, anchor="nw")
        self.instructions_label.grid(row=1, column=0, rowspan=4, padx=10, pady=5, sticky="nsew")

        # File Upload Button
        self.upload_button = tk.Button(self.root, text="Upload File", command=self.upload_file)
        self.upload_button.grid(row=1, column=1, columnspan=2, padx=10, pady=5)

        # File Path Label
        self.file_path_label = tk.Label(self.root, textvariable=self.file_path)
        self.file_path_label.grid(row=2, column=1, columnspan=2, padx=10, pady=5, sticky="w")

        # Input Textbox
        self.input_label = tk.Label(self.root, text="Input:")
        self.input_label.grid(row=3, column=1, padx=10, pady=5, sticky="w")
        self.input_textbox = tk.Text(self.root, height=10, width=50)
        self.input_textbox.grid(row=4, column=1, padx=10, pady=5, sticky="nsew")

        # Output Textbox
        self.output_label = tk.Label(self.root, text="Output:")
        self.output_label.grid(row=5, column=1, padx=10, pady=5, sticky="w")
        self.output_textbox = tk.Text(self.root, height=10, width=50)
        self.output_textbox.grid(row=6, column=1, padx=10, pady=5, sticky="nsew")

        # Execute Button
        self.execute_button = tk.Button(self.root, text="Execute Program", command=self.execute_program)
        self.execute_button.grid(row=7, column=1, columnspan=2, padx=10, pady=5)

        # Configure resizing behavior
        self.root.columnconfigure(0, weight=1)
        self.root.columnconfigure(1, weight=3)
        self.root.rowconfigure(4, weight=1)  # Make sure text areas expand with window size

    def upload_file(self):
        file_path = filedialog.askopenfilename()
        self.file_path.set(file_path)  # Update the label text to show the file path
        if not file_path:  # Reset the label if no file is selected
            self.file_path.set("No file selected")

    def execute_program(self):
        file_path = self.file_path.get()
        input_data = self.input_textbox.get("1.0", tk.END)

        if file_path:
            try:
                # Execute the C program with the file path and input file
                command = ["./vm_riskxvii", file_path]
                process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                output, _ = process.communicate(input_data.encode())
                self.output_textbox.delete("1.0", tk.END)
                self.output_textbox.insert(tk.END, output.decode())
            except Exception as e:
                self.output_textbox.delete("1.0", tk.END)
                self.output_textbox.insert(tk.END, "Error executing program: " + str(e))
        else:
            self.output_textbox.delete("1.0", tk.END)
            self.output_textbox.insert(tk.END, "Please upload a file.")

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
