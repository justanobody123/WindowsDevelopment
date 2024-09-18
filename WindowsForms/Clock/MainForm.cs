using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Clock
{
	public partial class MainForm : Form
	{
		bool controlsVisible;
		public MainForm()
		{
            controlsVisible = true;

            InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			//WM_CREATE
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
    }
}
