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
using System.Drawing.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Reflection.Emit;
using System.Reflection;

namespace Clock
{
	public partial class MainForm : Form
	{
		bool controlsVisible;
        //Класс, позволяющий использовать шрифты из ресурсов.
        //Своего рода массив загруженных шрифтов, которые можно вытащить через Families[x]
        PrivateFontCollection customFonts;
        Font digital7;

        public MainForm()
		{
            InitializeComponent();
            this.FormClosing += MainForm_FormClosing;
            this.Load += MainForm_Load;
            customFonts = new PrivateFontCollection();
            //using испольуется для работы с объектами, которые реализуют интерфейс IDisposable,
            //и он гарантирует, что ресурсы, занятые объектом,
            //будут высвобождены после завершения работы с ним.
            //Границы работы определяют скобки. Если не использовать using, то можно
            //вручную использовать на fontStream методы Dispose() или Close()

            //Читаем в поток памяти бинарные данные шрифта из ресурсов
            using (MemoryStream fontStream = new MemoryStream(Properties.Resources.DS_DIGII))
            {
                //Метод для выделения памяти для данных шрифта
                //Эта память НЕ УПРАВЛЯЕТСЯ GARBAGE COLLECTOR, поэтому она должна высвобождаться вручную (Marshal.FreeCoTaskMem(data);)
                IntPtr data = Marshal.AllocCoTaskMem((int)fontStream.Length);
                //Properties.Resources.DS_DIGII содержит данные шрифта в виде массива байтов

                //Мы не можем миновать посредника в лице массива байтов, потому что прямое чтение
                //в выделенный блок памяти не поддерживается потоками, ибо они оперируют только управляемой
                //памятью. А метод AddMemoryFont работает исключительно с неуправляемой памятью.
                byte[] fontData = new byte[fontStream.Length];
                //Читаем данные из потока в массив байтов
                fontStream.Read(fontData, 0, (int)fontStream.Length);
                //Копируем байты из массива в выделенный блок памяти, обращаясь к блоку через указатель
                Marshal.Copy(fontData, 0, data, (int)fontStream.Length);
                //Загружаем данные из выделенного блока памяти в коллекцию шрифтов
                customFonts.AddMemoryFont(data, (int)fontStream.Length);
                //Высвобождаем блок выделенной памяти
                Marshal.FreeCoTaskMem(data);

                //Нужно еще чуть-чуть теории

                //неуправляемый код работает вне CLR, и напрямую воздействует на ОС. Неуправляемый код работает
                //со следующими ключевыми словами:
                //unsafe — можно работать с указателями и неуправляемыми ресурсами, если блок кода помечен как unsafe.
                //Например:
                //unsafe
                //{
                //    int* p = &x;
                //}
                //Код, содержащий указатели, является неуправляемым, потому что он работает вне контроля Garbage Collector.
                //Работа с указателями и функциями Marshal
                //Класс Marshal содержит методы для работы с неуправляемой памятью —
                //операции с указателями и выделение неуправляемой памяти.
                //IntPtr:
                //Если в коде встречается IntPtr, это может означать, что происходит работа с указателями
                //или дескрипторами, которые указывают на неуправляемую память.
                //Но IntPtr может также работать и с управляемой памятью.

                //Разница между управляемой и неуправляемой памятью:

                //Управляемая память управляется CLR с использованием Garbage Collector
                //CLR автоматически выделяет и освобождает память, снижая риск утечек памяти и обращения к невалидному указателю.
                //Пример: объекты, созданные через операторы new, являются управляемыми,
                //и их память освобождается автоматически, когда объект больше не используется.

                //Неуправляемая память не контролируется Garbage Collector, поэтому выделять и освобождать ее
                //придется вручную. Например, через функции Marshal.AllocCoTaskMem() и Marshal.FreeCoTaskMem().
                //Неуправляемая память часто используется для взаимодействия
                //с нативными API или внешними библиотеками, которые работают вне CLR.

            }
            digital7 = new Font(customFonts.Families[0], Font.Size);
            //labelTime.Font = digital7;
            labelTime.UseCompatibleTextRendering = true;
            this.StartPosition = FormStartPosition.Manual;

			int startX = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Right - this.Right - 25;
			int startY = 25;
            this.Location = new Point(startX, startY);
            
		}

		private void MainForm_Load(object sender, EventArgs e)
		{

            if (Properties.Settings.Default.FontName == digital7.Name)
            {
                labelTime.Font = digital7;
            }
            else if (!string.IsNullOrEmpty(Properties.Settings.Default.FontName))
            {
                labelTime.Font = new Font(Properties.Settings.Default.FontName, labelTime.Font.Size);
            }
            SetControlsVisibility(Properties.Settings.Default.ShowControls);
            labelTime.BackColor = Properties.Settings.Default.BackgroundColor;
            labelTime.ForeColor = Properties.Settings.Default.ForegroundColor;
            cbShowDate.Checked = Properties.Settings.Default.ShowDate;
            if (Properties.Settings.Default.AppIcon == "WindowIcon")
            {
                this.Icon = Properties.Resources.WindowIcon;
                notifyIcon1.Icon = Properties.Resources.WindowIcon;
            }
            else
            {
                this.Icon = Properties.Resources.ClockIcon;
                notifyIcon1.Icon = Properties.Resources.ClockIcon;
            }

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

        private void digital7ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (labelTime.Font.Name != digital7.Name)
            {
                labelTime.Font = digital7;
            }
        }

        private void sansSerifToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (labelTime.Font.Name != "Microsoft Sans Serif")
            {
                //Вроде как из-за того, что встроенные шрифты управляются системой, можно их плодись с помощью new.
                //Звучит как лажа, но у меня пока нет времени, чтобы попытаться это опровергнуть.
                labelTime.Font = new Font("Microsoft Sans Serif", labelTime.Font.Size);
            }
        }

        private void BodoniMTToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(labelTime.Font.Name != "Bodoni MT")
            {
                //Вроде как из-за того, что встроенные шрифты управляются системой, можно их плодись с помощью new.
                //Звучит как лажа, но у меня пока нет времени, чтобы попытаться это опровергнуть.
                labelTime.Font = new Font("Bodoni MT", labelTime.Font.Size);
            }
            
        }

        private void arialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (labelTime.Font.Name != "Arial")
            {
                //Вроде как из-за того, что встроенные шрифты управляются системой, можно их плодись с помощью new.
                //Звучит как лажа, но у меня пока нет времени, чтобы попытаться это опровергнуть.
                labelTime.Font = new Font("Arial", labelTime.Font.Size);
            }
        }
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.FontName = labelTime.Font.Name;
            Properties.Settings.Default.BackgroundColor = labelTime.BackColor;
            Properties.Settings.Default.ForegroundColor = labelTime.ForeColor;
            Properties.Settings.Default.ShowControls = controlsVisible;
            Properties.Settings.Default.ShowDate = cbShowDate.Checked;
            Properties.Settings.Default.Save();
        }

        private void windowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Icon newIcon = Properties.Resources.WindowIcon;
            if (notifyIcon1.Icon != newIcon)
            {
                notifyIcon1.Icon = newIcon;
                this.Icon = newIcon;
                Properties.Settings.Default.AppIcon = "WindowIcon";
            }
        }

        private void clockToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Icon newIcon = Properties.Resources.ClockIcon;
            if (notifyIcon1.Icon != newIcon)
            {
                notifyIcon1.Icon = newIcon;
                this.Icon = newIcon;
                Properties.Settings.Default.AppIcon = "ClockIcon";
            }
        }
    }
	
}
