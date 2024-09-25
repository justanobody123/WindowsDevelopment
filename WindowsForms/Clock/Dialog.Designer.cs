namespace Clock
{
    partial class Dialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Dialog));
            this.labelDialog = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelDialog
            // 
            this.labelDialog.AutoSize = true;
            this.labelDialog.Font = new System.Drawing.Font("Century Schoolbook", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDialog.Location = new System.Drawing.Point(97, 43);
            this.labelDialog.Name = "labelDialog";
            this.labelDialog.Size = new System.Drawing.Size(142, 22);
            this.labelDialog.TabIndex = 0;
            this.labelDialog.Text = "Wake up, Neo...";
            // 
            // Dialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(334, 101);
            this.Controls.Add(this.labelDialog);
            this.ForeColor = System.Drawing.Color.LightGreen;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Dialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Alarm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Dialog_FormClosing);
            this.Load += new System.EventHandler(this.Dialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelDialog;
    }
}