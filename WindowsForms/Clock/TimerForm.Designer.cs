namespace Clock
{
    partial class TimerForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnOk = new System.Windows.Forms.Button();
            this.dateTimePickerTimer = new System.Windows.Forms.DateTimePicker();
            this.labelTimerInvitation = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(59, 145);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 0;
            this.btnOk.Text = "OK";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // dateTimePickerTimer
            // 
            this.dateTimePickerTimer.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerTimer.Location = new System.Drawing.Point(56, 75);
            this.dateTimePickerTimer.Name = "dateTimePickerTimer";
            this.dateTimePickerTimer.ShowUpDown = true;
            this.dateTimePickerTimer.Size = new System.Drawing.Size(86, 22);
            this.dateTimePickerTimer.TabIndex = 0;
            this.dateTimePickerTimer.Value = new System.DateTime(2024, 9, 26, 0, 0, 0, 0);
            // 
            // labelTimerInvitation
            // 
            this.labelTimerInvitation.AutoSize = true;
            this.labelTimerInvitation.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F);
            this.labelTimerInvitation.Location = new System.Drawing.Point(12, 32);
            this.labelTimerInvitation.Name = "labelTimerInvitation";
            this.labelTimerInvitation.Size = new System.Drawing.Size(181, 20);
            this.labelTimerInvitation.TabIndex = 1;
            this.labelTimerInvitation.Text = "Установите таймер:";
            // 
            // TimerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(196, 194);
            this.Controls.Add(this.labelTimerInvitation);
            this.Controls.Add(this.dateTimePickerTimer);
            this.Controls.Add(this.btnOk);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "TimerForm";
            this.Text = "TimerForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.DateTimePicker dateTimePickerTimer;
        private System.Windows.Forms.Label labelTimerInvitation;
    }
}