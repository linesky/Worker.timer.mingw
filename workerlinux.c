#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//sudo apt-get install libgtk-3-dev
//gcc main.c -o worker_management `pkg-config --cflags --libs gtk+-3.0`

// Callback functions
void add_worker(GtkWidget *widget, gpointer data);
void open_file(GtkWidget *widget, gpointer data);
void generate_report(GtkWidget *widget, gpointer data);
void list_dates(GtkWidget *widget, gpointer data);
void list_jobs(GtkWidget *widget, gpointer data);

// Global widgets
GtkWidget *worker_entry, *workin_entry, *workout_entry, *job_entry, *reports_textview, *filename_entry;

int main(int argc, char *argv[])
{
    GtkWidget *window, *grid, *add_button, *open_button, *report_button, *listdates_button, *listjobs_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Worker Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    GdkRGBA yellow = {1, 1, 0, 1};
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &yellow);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    worker_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(worker_entry), "Worker ID");
    gtk_grid_attach(GTK_GRID(grid), worker_entry, 0, 0, 1, 1);

    workin_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(workin_entry), "Work In (YYYY/MM/DD HH:MM)");
    gtk_grid_attach(GTK_GRID(grid), workin_entry, 0, 1, 1, 1);

    workout_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(workout_entry), "Work Out (YYYY/MM/DD HH:MM)");
    gtk_grid_attach(GTK_GRID(grid), workout_entry, 0, 2, 1, 1);

    job_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(job_entry), "Job ID");
    gtk_grid_attach(GTK_GRID(grid), job_entry, 0, 3, 1, 1);

    reports_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(reports_textview), FALSE);
    gtk_grid_attach(GTK_GRID(grid), reports_textview, 1, 0, 1, 4);

    filename_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(filename_entry), "Report Filename");
    gtk_grid_attach(GTK_GRID(grid), filename_entry, 0, 4, 1, 1);

    add_button = gtk_button_new_with_label("Add Worker");
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_worker), NULL);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 5, 1, 1);

    open_button = gtk_button_new_with_label("Open File");
    g_signal_connect(open_button, "clicked", G_CALLBACK(open_file), NULL);
    gtk_grid_attach(GTK_GRID(grid), open_button, 1, 5, 1, 1);

    report_button = gtk_button_new_with_label("Generate Report");
    g_signal_connect(report_button, "clicked", G_CALLBACK(generate_report), NULL);
    gtk_grid_attach(GTK_GRID(grid), report_button, 0, 6, 1, 1);

    listdates_button = gtk_button_new_with_label("List Dates");
    g_signal_connect(listdates_button, "clicked", G_CALLBACK(list_dates), NULL);
    gtk_grid_attach(GTK_GRID(grid), listdates_button, 1, 6, 1, 1);

    listjobs_button = gtk_button_new_with_label("List Jobs");
    g_signal_connect(listjobs_button, "clicked", G_CALLBACK(list_jobs), NULL);
    gtk_grid_attach(GTK_GRID(grid), listjobs_button, 0, 7, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void add_worker(GtkWidget *widget, gpointer data)
{
    const char *worker = gtk_entry_get_text(GTK_ENTRY(worker_entry));
    const char *work_in = gtk_entry_get_text(GTK_ENTRY(workin_entry));
    const char *work_out = gtk_entry_get_text(GTK_ENTRY(workout_entry));
    const char *job = gtk_entry_get_text(GTK_ENTRY(job_entry));
    FILE *file = fopen("worker.dat", "a");

    if (!file)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to open worker.dat!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    fprintf(file, "%s,%s,%s,%s\n", worker, work_in, work_out, job);
    fclose(file);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Worker added successfully!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void open_file(GtkWidget *widget, gpointer data)
{
    system("xdg-open worker.dat");
}

void generate_report(GtkWidget *widget, gpointer data)
{
    const char *worker = gtk_entry_get_text(GTK_ENTRY(worker_entry));
    char buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");
    struct tm tm_in, tm_out;
    time_t time_in, time_out;

    if (!file)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to open worker.dat!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    GtkTextBuffer *buffer_widget = gtk_text_view_get_buffer(GTK_TEXT_VIEW(reports_textview));
    gtk_text_buffer_set_text(buffer_widget, "", -1);

    while (fgets(buffer, 1024, file))
    {
        char file_worker[10], work_in[20], work_out[20], job[10];
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^\n]", file_worker, work_in, work_out, job);

        if (strcmp(file_worker, worker) == 0)
        {
            strptime(work_in, "%Y/%m/%d %H:%M", &tm_in);
            strptime(work_out, "%Y/%m/%d %H:%M", &tm_out);

            time_in = mktime(&tm_in);
            time_out = mktime(&tm_out);

            double hours_worked = difftime(time_out, time_in) / 3600;

            char line[256];
            sprintf(line, "%s,%.2f,%s\n", worker, hours_worked, job);
            strcat(output, line);
        }
    }
    fclose(file);
    gtk_text_buffer_set_text(buffer_widget, output, -1);
}

void list_dates(GtkWidget *widget, gpointer data)
{
    const char *work_in = gtk_entry_get_text(GTK_ENTRY(workin_entry));
    char buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");
    struct tm tm_in;

    if (!file)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to open worker.dat!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    strptime(work_in, "%Y/%m/%d %H:%M", &tm_in);
    time_t time_in = mktime(&tm_in);

    GtkTextBuffer *buffer_widget = gtk_text_view_get_buffer(GTK_TEXT_VIEW(reports_textview));
    gtk_text_buffer_set_text(buffer_widget, "", -1);

    while (fgets(buffer, 1024, file))
    {
        char file_worker[10], file_work_in[20], file_work_out[20], file_job[10];
        struct tm file_tm_in;
        time_t file_time_in;

        sscanf(buffer, "%[^,],%[^,],%[^,],%[^\n]", file_worker, file_work_in, file_work_out, file_job);
        strptime(file_work_in, "%Y/%m/%d %H:%M", &file_tm_in);
        file_time_in = mktime(&file_tm_in);

        if (difftime(file_time_in, time_in) >= 0)
        {
            char line[256];
            sprintf(line, "%s,%s,%s\n", file_worker, file_work_in, file_job);
            strcat(output, line);
        }
    }
    fclose(file);
    gtk_text_buffer_set_text(buffer_widget, output, -1);
}

void list_jobs(GtkWidget *widget, gpointer data)
{
    const char *job = gtk_entry_get_text(GTK_ENTRY(job_entry));
    char buffer[1024], output[2048] = "";
    FILE *file = fopen("worker.dat", "r");

    if (!file)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Failed to open worker.dat!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    GtkTextBuffer *buffer_widget = gtk_text_view_get_buffer(GTK_TEXT_VIEW(reports_textview));
    gtk_text_buffer_set_text(buffer_widget, "", -1);

    while (fgets(buffer, 1024, file))
    {
        char file_worker[10], file_work_in[20], file_work_out[20], file_job[10];
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^\n]", file_worker, file_work_in, file_work_out, file_job);

        if (strcmp(file_job, job) == 0)
        {
            char line[256];
            sprintf(line, "%s,%s,%s\n", file_worker, file_work_in, file_job);
            strcat(output, line);
        }
    }
    fclose(file);
    gtk_text_buffer_set_text(buffer_widget, output, -1);
}

