import customtkinter


class MyCheckboxFrame(customtkinter.CTkFrame):
    def __init__(self, master, title, values):
        super().__init__(master)
        self.grid_columnconfigure(0, weight=1)
        self.values = values
        self.title = title
        self.checkboxes = []

        self.title = customtkinter.CTkLabel(
            self, text=self.title, fg_color="gray30", corner_radius=6
        )
        self.title.grid(row=0, column=0, padx=10, pady=(10, 0), sticky="ew")

        for i, value in enumerate(self.values):
            checkbox = customtkinter.CTkCheckBox(self, text=value)
            checkbox.grid(row=i + 1, column=0, padx=10, pady=(10, 0), sticky="w")
            self.checkboxes.append(checkbox)

    def get(self):
        checked_checkboxes = []
        for checkbox in self.checkboxes:
            if checkbox.get() == 1:
                checked_checkboxes.append(checkbox.cget("text"))
        return checked_checkboxes


class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()

        self.title("my app")
        self.geometry("400x180")
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(0, weight=1)

        self.checkbox_frame_1 = MyCheckboxFrame(
            self, "Values", values=["value 1", "value 2", "value 3"]
        )
        self.checkbox_frame_1.grid(
            row=0, column=0, padx=10, pady=(10, 0), sticky="nsew"
        )
        self.checkbox_frame_2 = MyCheckboxFrame(
            self, "Options", values=["option 1", "option 2"]
        )
        self.checkbox_frame_2.grid(
            row=0, column=1, padx=10, pady=(10, 0), sticky="nsew"
        )
        self.checkbox_frame_2.configure(fg_color="transparent")

        self.button = customtkinter.CTkButton(
            self, text="my button", command=self.button_callback
        )
        self.button.grid(row=3, column=0, padx=10, pady=10, sticky="ew", columnspan=2)

    def button_callback(self):
        print("button pressed")
        print("checked checkbox_frame_1: ", self.checkbox_frame_1.get())
        print("checked checkbox_frame_2: ", self.checkbox_frame_2.get())


app = App()
app.mainloop()