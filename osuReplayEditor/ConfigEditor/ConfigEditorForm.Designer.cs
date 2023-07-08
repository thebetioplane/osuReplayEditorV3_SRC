namespace osuReplayEditor.ConfigEditor
{
    partial class ConfigEditorForm
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
            this.osuDbPathTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.songFolderPathTextbox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.replayFolderPathTextbox = new System.Windows.Forms.TextBox();
            this.accept_btn = new System.Windows.Forms.Button();
            this.edit_in_notepad_btn = new System.Windows.Forms.Button();
            this.cancel_btn = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.selectOsuFolderBtn = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // osuDbPathTextBox
            // 
            this.osuDbPathTextBox.Location = new System.Drawing.Point(84, 135);
            this.osuDbPathTextBox.Name = "osuDbPathTextBox";
            this.osuDbPathTextBox.Size = new System.Drawing.Size(264, 20);
            this.osuDbPathTextBox.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 138);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(66, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "osu!.db path";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(9, 109);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(336, 23);
            this.label2.TabIndex = 2;
            this.label2.Text = "Or configure manually";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 164);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Song folder";
            // 
            // songFolderPathTextbox
            // 
            this.songFolderPathTextbox.Location = new System.Drawing.Point(84, 161);
            this.songFolderPathTextbox.Name = "songFolderPathTextbox";
            this.songFolderPathTextbox.Size = new System.Drawing.Size(264, 20);
            this.songFolderPathTextbox.TabIndex = 3;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 190);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(69, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Replay folder";
            // 
            // replayFolderPathTextbox
            // 
            this.replayFolderPathTextbox.Location = new System.Drawing.Point(84, 187);
            this.replayFolderPathTextbox.Name = "replayFolderPathTextbox";
            this.replayFolderPathTextbox.Size = new System.Drawing.Size(264, 20);
            this.replayFolderPathTextbox.TabIndex = 5;
            // 
            // accept_btn
            // 
            this.accept_btn.Location = new System.Drawing.Point(12, 216);
            this.accept_btn.Name = "accept_btn";
            this.accept_btn.Size = new System.Drawing.Size(75, 23);
            this.accept_btn.TabIndex = 7;
            this.accept_btn.Text = "Accept";
            this.accept_btn.UseVisualStyleBackColor = true;
            this.accept_btn.Click += new System.EventHandler(this.accept_btn_Click);
            // 
            // edit_in_notepad_btn
            // 
            this.edit_in_notepad_btn.Location = new System.Drawing.Point(105, 216);
            this.edit_in_notepad_btn.Name = "edit_in_notepad_btn";
            this.edit_in_notepad_btn.Size = new System.Drawing.Size(150, 23);
            this.edit_in_notepad_btn.TabIndex = 8;
            this.edit_in_notepad_btn.Text = "Edit in Notepad";
            this.edit_in_notepad_btn.UseVisualStyleBackColor = true;
            this.edit_in_notepad_btn.Click += new System.EventHandler(this.edit_in_notepad_btn_Click);
            // 
            // cancel_btn
            // 
            this.cancel_btn.Location = new System.Drawing.Point(273, 216);
            this.cancel_btn.Name = "cancel_btn";
            this.cancel_btn.Size = new System.Drawing.Size(75, 23);
            this.cancel_btn.TabIndex = 9;
            this.cancel_btn.Text = "Cancel";
            this.cancel_btn.UseVisualStyleBackColor = true;
            this.cancel_btn.Click += new System.EventHandler(this.cancel_btn_Click);
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(12, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(336, 32);
            this.label6.TabIndex = 11;
            this.label6.Text = "osu!ReplayEditor v3 Config Setup";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // selectOsuFolderBtn
            // 
            this.selectOsuFolderBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.selectOsuFolderBtn.Location = new System.Drawing.Point(12, 54);
            this.selectOsuFolderBtn.Name = "selectOsuFolderBtn";
            this.selectOsuFolderBtn.Size = new System.Drawing.Size(336, 33);
            this.selectOsuFolderBtn.TabIndex = 12;
            this.selectOsuFolderBtn.Text = "Select osu! Folder or Shortcut";
            this.selectOsuFolderBtn.UseVisualStyleBackColor = true;
            this.selectOsuFolderBtn.Click += new System.EventHandler(this.selectOsuFolderBtn_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.CheckFileExists = false;
            this.openFileDialog.CheckPathExists = false;
            this.openFileDialog.FileName = "osu!.exe";
            this.openFileDialog.Filter = "osu files|*.exe;*.db";
            this.openFileDialog.ValidateNames = false;
            this.openFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog_FileOk);
            // 
            // ConfigEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(360, 253);
            this.Controls.Add(this.selectOsuFolderBtn);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.cancel_btn);
            this.Controls.Add(this.edit_in_notepad_btn);
            this.Controls.Add(this.accept_btn);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.replayFolderPathTextbox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.songFolderPathTextbox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.osuDbPathTextBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "ConfigEditorForm";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "osu!ReplayEditor v3 Config Setup";
            this.Shown += new System.EventHandler(this.ConfigEditorForm_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox osuDbPathTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox songFolderPathTextbox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox replayFolderPathTextbox;
        private System.Windows.Forms.Button accept_btn;
        private System.Windows.Forms.Button edit_in_notepad_btn;
        private System.Windows.Forms.Button cancel_btn;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button selectOsuFolderBtn;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}