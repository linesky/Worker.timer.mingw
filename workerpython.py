import tkinter as tk
from tkinter import messagebox, filedialog
from datetime import datetime
import csv
import os

# Função para adicionar trabalhador
def add_worker():
    worker = worker_entry.get()
    work_in = workin_entry.get()
    work_out = workout_entry.get()
    job = job_entry.get()

    with open('worker.dat', 'a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow([worker, work_in, work_out, job])

    messagebox.showinfo("Success", "Worker added successfully!")

# Função para abrir o arquivo CSV
def open_file():
    if os.path.exists('worker.dat'):
        os.system(f'notepad worker.dat')
    else:
        messagebox.showerror("Error", "worker.dat not found!")

# Função para gerar relatório
def generate_report():
    worker = worker_entry.get()
    output = ""
    
    if not os.path.exists('worker.dat'):
        messagebox.showerror("Error", "worker.dat not found!")
        return

    with open('worker.dat', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            file_worker, work_in, work_out, job = row
            if file_worker == worker:
                time_in = datetime.strptime(work_in, "%Y/%m/%d %H:%M")
                time_out = datetime.strptime(work_out, "%Y/%m/%d %H:%M")
                hours_worked = (time_out - time_in).total_seconds() / 3600
                output += f"{worker},{hours_worked:.2f},{job}\n"
    
    reports_text.delete('1.0', tk.END)
    reports_text.insert(tk.END, output)

# Função para listar datas
def list_dates():
    work_in = workin_entry.get()
    time_in = datetime.strptime(work_in, "%Y/%m/%d %H:%M")
    output = ""
    
    if not os.path.exists('worker.dat'):
        messagebox.showerror("Error", "worker.dat not found!")
        return

    with open('worker.dat', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            file_worker, file_work_in, file_work_out, file_job = row
            file_time_in = datetime.strptime(file_work_in, "%Y/%m/%d %H:%M")
            if file_time_in >= time_in:
                output += f"{file_worker},{file_work_in},{file_job}\n"
    
    reports_text.delete('1.0', tk.END)
    reports_text.insert(tk.END, output)

# Função para listar trabalhos
def list_jobs():
    job = job_entry.get()
    output = ""
    
    if not os.path.exists('worker.dat'):
        messagebox.showerror("Error", "worker.dat not found!")
        return

    with open('worker.dat', 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            file_worker, file_work_in, file_work_out, file_job = row
            if file_job == job:
                output += f"{file_worker},{file_work_in},{file_job}\n"
    
    reports_text.delete('1.0', tk.END)
    reports_text.insert(tk.END, output)

# Criando a janela principal
root = tk.Tk()
root.title("Worker Management")
root.configure(bg='yellow')
root.geometry("600x400")

# Campos de entrada
worker_entry = tk.Entry(root)
worker_entry.insert(0, "Worker ID")
worker_entry.grid(row=0, column=0, padx=10, pady=5)

workin_entry = tk.Entry(root)
workin_entry.insert(0, "Work In (YYYY/MM/DD HH:MM)")
workin_entry.grid(row=1, column=0, padx=10, pady=5)

workout_entry = tk.Entry(root)
workout_entry.insert(0, "Work Out (YYYY/MM/DD HH:MM)")
workout_entry.grid(row=2, column=0, padx=10, pady=5)

job_entry = tk.Entry(root)
job_entry.insert(0, "Job ID")
job_entry.grid(row=3, column=0, padx=10, pady=5)

# Campo de texto para relatórios
reports_text = tk.Text(root, width=40, height=15)
reports_text.grid(row=0, column=1, rowspan=6, padx=10, pady=5)

filename_entry = tk.Entry(root)
filename_entry.insert(0, "Report Filename")
filename_entry.grid(row=4, column=0, padx=10, pady=5)

# Botões
add_button = tk.Button(root, text="Add Worker", command=add_worker)
add_button.grid(row=5, column=0, padx=10, pady=5)

open_button = tk.Button(root, text="Open File", command=open_file)
open_button.grid(row=5, column=1, padx=10, pady=5)

report_button = tk.Button(root, text="Generate Report", command=generate_report)
report_button.grid(row=6, column=0, padx=10, pady=5)

listdates_button = tk.Button(root, text="List Dates", command=list_dates)
listdates_button.grid(row=6, column=1, padx=10, pady=5)

listjobs_button = tk.Button(root, text="List Jobs", command=list_jobs)
listjobs_button.grid(row=7, column=0, padx=10, pady=5)

# Iniciando o loop principal da janela
root.mainloop()

