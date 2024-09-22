using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Clock
{
	public partial class MainForm : Form
	{
		bool controlsVisible;
        public Label LabelTime
        {
            get => labelTime;
        }
		public MainForm()
		{
            AllocConsole();
            InitializeComponent();
            this.FormClosing += MainForm_Save;
            SetControlsVisibility(false);
			this.StartPosition = FormStartPosition.Manual;

			int startX = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Right - this.Right - 25;
			int startY = 25;
            this.Location = new Point(startX, startY);


            
            CreateCustomFont(); 
		}
        void CreateCustomFont()
        {
            Console.WriteLine(Directory.GetCurrentDirectory());
            Directory.SetCurrentDirectory("..\\..\\Fonts");
            Console.WriteLine(Directory.GetCurrentDirectory());

            PrivateFontCollection pfc = new PrivateFontCollection();
            pfc.AddFontFile("Terminat.ttf");
            Font font = new Font(pfc.Families[0], labelTime.Font.Size);
            pfc.Dispose();
            labelTime.Font = font;
        }
        private void MainForm_Load(object sender, EventArgs e)
		{
            //WM_CREATE
            SetControlsVisibility(Properties.Settings.Default.ShowControls);
            cbShowDate.Checked = Properties.Settings.Default.ShowDate;
            LabelTime.ForeColor = Properties.Settings.Default.ForegroundColor;
            LabelTime.BackColor = Properties.Settings.Default.BackgroundColor;
            Console.WriteLine(Directory.GetCurrentDirectory());
            Console.WriteLine(Properties.Settings.Default.FontName);
            PrivateFontCollection pfc = new PrivateFontCollection();
            pfc.AddFontFile(Properties.Settings.Default.FontName);
            Console.WriteLine(pfc.Families[0].ToString());
            Font font = new Font(pfc.Families[0], Properties.Settings.Default.FontSize);
            LabelTime.Font = font;
            pfc.Dispose();
        }

		private void timer1_Tick(object sender, EventArgs e)
		{
			labelTime.Text = DateTime.Now.ToString("hh:mm:ss tt");
			if (cbShowDate.Checked) 
			{
				labelTime.Text += $"\n{DateTime.Now.ToString("yyyy.MM.dd")}";
			}
		}
		private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			//Под this понимается объект типа MainForm, к которому применяется метод, sender - тот, для кого было вызвано событие (notifyIcon1)
			this.WindowState = FormWindowState.Normal;
			this.ShowInTaskbar = true;
		}
		//Переопределяем метод из базового класса Form, отсюда и сигнатура
		protected override void OnResize(EventArgs e)
		{
			if (this.WindowState == FormWindowState.Minimized)
			{
				this.ShowInTaskbar = false;
			}
		}
		private void btnHideControls_Click(object sender, EventArgs e)
		{

			SetControlsVisibility(false);

        }
		void SetControlsVisibility(bool visible)
		{
            this.FormBorderStyle = visible ? FormBorderStyle.Sizable : FormBorderStyle.None;
            this.TransparencyKey = visible ? Color.Empty : this.BackColor;
            labelTime.BackColor = visible ? this.BackColor : Color.LightBlue;
            this.cbShowDate.Visible = visible;
            this.TopMost = !visible;
            btnHideControls.Visible = visible;
            this.ShowInTaskbar = visible;
			showControlsToolStripMenuItem.Checked = visible;
			this.controlsVisible = visible;

        }

        private void labelTime_MouseHover(object sender, EventArgs e)
        {
            SetControlsVisibility(true);
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
			this.Close();
        }

        private void showControlsToolStripMenuItem_Click(object sender, EventArgs e)
        {
			SetControlsVisibility(!controlsVisible);
        }

        private void showDateToolStripMenuItem_Click(object sender, EventArgs e)
        {

			cbShowDate.Checked = showDateToolStripMenuItem.Checked;
        }

        private void cbShowDate_CheckedChanged(object sender, EventArgs e)
        {
			this.controlsVisible = showControlsToolStripMenuItem.Checked;
			SetControlsVisibility(controlsVisible);
        }

        private void backgroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
			ColorDialog dialog = new ColorDialog();
			if (dialog.ShowDialog() == DialogResult.OK)
			{
				labelTime.BackColor = dialog.Color;
			}
			
        }

        private void foregroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dialog = new ColorDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                labelTime.ForeColor = dialog.Color;
            }
        }

        private void cbShowDate_Enter(object sender, EventArgs e)
        {
			labelTime.Focus();
        }
        [DllImport("kernel32.dll")]
        static extern bool AllocConsole();

        private void chooseFontToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChooseFont dialog = new ChooseFont(this);
            dialog.Show();
        }
        private void MainForm_Save(object sender, EventArgs e)
        {
            Properties.Settings.Default.ForegroundColor = LabelTime.ForeColor;
            Properties.Settings.Default.BackgroundColor = LabelTime.BackColor;
            Properties.Settings.Default.ShowControls = controlsVisible;
            Properties.Settings.Default.ShowDate = cbShowDate.Checked;
            Properties.Settings.Default.Save();
        }
    }
}
