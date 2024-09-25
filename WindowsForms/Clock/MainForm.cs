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
using System.Threading;
using System.Reflection.Emit;
namespace Clock
{
	public partial class MainForm : Form
	{
		bool controlsVisible;
        ChooseFont chooseFontDialog;
        AlarmClock alarmClock;
        public System.Windows.Forms.Label LabelTime
        {
            get => labelTime;
        }
        public string AlarmTime
        {
            get; set;
        }
        public NotifyIcon NotifyIcon1
        {
            get => notifyIcon1;
            set => notifyIcon1 = value;
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
            chooseFontDialog = new ChooseFont(this);
            alarmClock = new AlarmClock(this);
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
            pinToolStripMenuItem.Checked = Properties.Settings.Default.Pin;
            cbPin.Checked = Properties.Settings.Default.Pin;
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
            if (!string.IsNullOrEmpty(AlarmTime))
            {
                Console.WriteLine("Timer: " + labelTime.Text);
                Console.WriteLine("Alarm: " + AlarmTime);
                //Проверка на совпадение времени таймера и времени будильника
                if (labelTime.Text.Contains(AlarmTime))
                {
                    //если да
                    //очистка строки
                    AlarmTime = string.Empty;
                    Console.WriteLine("Будильник сработал!");
                    Dialog dialog = new Dialog();
                    dialog.ShowDialog(this);
                    NotifyIcon1.Text = "mainForm";
                }


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
            //this.TopMost = !visible;
            btnHideControls.Visible = visible;
            this.ShowInTaskbar = visible;
			showControlsToolStripMenuItem.Checked = visible;
			this.controlsVisible = visible;
            this.cbPin.Visible = visible;

        }

        private void labelTime_MouseHover(object sender, EventArgs e)
        {
            
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
            
            chooseFontDialog.Show();
        }
        private void MainForm_Save(object sender, EventArgs e)
        {
            Properties.Settings.Default.ForegroundColor = LabelTime.ForeColor;
            Properties.Settings.Default.BackgroundColor = LabelTime.BackColor;
            Properties.Settings.Default.ShowControls = controlsVisible;
            Properties.Settings.Default.ShowDate = cbShowDate.Checked;
            Properties.Settings.Default.Pin = pinToolStripMenuItem.Checked;
            Properties.Settings.Default.Save();
        }

        private void pinToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.TopMost = pinToolStripMenuItem.Checked;
            cbPin.Checked = pinToolStripMenuItem.Checked;
        }

        private void notifyIcon1_Click(object sender, EventArgs e)
        {
            if (this.TopMost == false)
            {
                this.TopMost = true;
                this.TopMost = false;
                
            }
        }
        private void cbPin_CheckedChanged(object sender, EventArgs e)
        {
            if (cbPin.Checked) 
            {
                cbPin.BackgroundImage = Properties.Resources.pinned.ToBitmap();
            }
            else 
            {
                cbPin.BackgroundImage = Properties.Resources.not_pinned.ToBitmap();
            }
            pinToolStripMenuItem.Checked = cbPin.Checked;
        }

        private void setAnAlarmToolStripMenuItem_Click(object sender, EventArgs e)
        {
            alarmClock.Show();
        }
    }
}
