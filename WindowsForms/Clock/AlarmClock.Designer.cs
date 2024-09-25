namespace Clock
{
    partial class AlarmClock
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AlarmClock));
            this.alarmTime = new System.Windows.Forms.DateTimePicker();
            this.labelAlarm = new System.Windows.Forms.Label();
            this.btnOkAlarm = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // alarmTime
            // 
            this.alarmTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.alarmTime.Location = new System.Drawing.Point(89, 75);
            this.alarmTime.Name = "alarmTime";
            this.alarmTime.ShowUpDown = true;
            this.alarmTime.Size = new System.Drawing.Size(109, 22);
            this.alarmTime.TabIndex = 0;
            this.alarmTime.Value = new System.DateTime(2024, 9, 25, 0, 0, 0, 0);
            // 
            // labelAlarm
            // 
            this.labelAlarm.AutoSize = true;
            this.labelAlarm.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.labelAlarm.Location = new System.Drawing.Point(66, 37);
            this.labelAlarm.Name = "labelAlarm";
            this.labelAlarm.Size = new System.Drawing.Size(155, 20);
            this.labelAlarm.TabIndex = 1;
            this.labelAlarm.Text = "Выберите время:";
            // 
            // btnOkAlarm
            // 
            this.btnOkAlarm.Location = new System.Drawing.Point(108, 117);
            this.btnOkAlarm.Name = "btnOkAlarm";
            this.btnOkAlarm.Size = new System.Drawing.Size(75, 23);
            this.btnOkAlarm.TabIndex = 2;
            this.btnOkAlarm.Text = "OK";
            this.btnOkAlarm.UseVisualStyleBackColor = true;
            this.btnOkAlarm.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // AlarmClock
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 161);
            this.Controls.Add(this.btnOkAlarm);
            this.Controls.Add(this.labelAlarm);
            this.Controls.Add(this.alarmTime);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "AlarmClock";
            this.Text = "Будильник";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DateTimePicker alarmTime;
        private System.Windows.Forms.Label labelAlarm;
        private System.Windows.Forms.Button btnOkAlarm;
    }
}