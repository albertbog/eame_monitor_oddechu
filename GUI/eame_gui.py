import sys, os
from datetime import datetime

from PyQt6.QtCore import QDate, QTime, QDateTime, Qt
from PyQt6.QtWidgets import QApplication, QMainWindow, QFileDialog, QMessageBox
from PyQt6.QtGui import QAction
import Analysis




#-----funkcja pomocnicza do kompilacji UI i import formatki graficznej mainwindow.ui-----#
def compileUi(src_file, dst_file):
	from PyQt6.uic import compileUi
	src_fileh = open(os.path.dirname(__file__)+'\\'+src_file, 'r')
	dst_fileh = open(os.path.dirname(__file__)+'\\'+dst_file, 'w')
	compileUi(src_fileh, dst_fileh)
	src_fileh.close()
	dst_fileh.close()

compileUi('mainwindow.ui', 'mainwindow_ui.py')
from mainwindow_ui import Ui_MainWindow
#-----funkcja pomocnicza do kompilacji UI i import formatki graficznej mainwindow.ui-----#

isEmpty = 0

#-----klasa GUI / glowne okno-----#
class MainWindow(QMainWindow, Ui_MainWindow):

	#---konstruktor---#
	def __init__(self, *args, obj=None, **kwargs):
		super(MainWindow, self).__init__(*args, **kwargs)
		self.setupUi(self)

		self.label_pb.clicked.connect(self.obslButtonLabel) #laczy nacisniecie 'AKT.CZAS' (label_pb) z metoda 'obslButtonLabel'

		self.lineEdit_pb.clicked.connect(self.obslButtonLineEdit) #laczy nacisniecie 'WPROWADZ' (lineEdit_pb) z metoda 'obslButtonLineEdit'

		self.radioButtonA.toggled.connect(self.obslRadioButton) #laczy wybranie RadioButton 'A' (radioButtonA) z metoda 'obslRadioButton'
		self.radioButtonB.toggled.connect(self.obslRadioButton) #laczy wybranie RadioButton 'B' (radioButtonA) z metoda 'obslRadioButton'

		self.comboBox.currentIndexChanged.connect(self.obslComboBox) #laczy zmiane wyboru ComboBox (comboBox) z metoda 'obslComboBox'

		self.slider_sld.valueChanged.connect(self.obslSlider) #laczy zmiane wartosci slider z metoda 'obslSlider'

		self.textBrowser_pb.clicked.connect(self.obslLoadFile) #laczy nacisniecie 'LADUJ PLIK' (textBrowser_pb) z metoda 'obslLoadFile'
		self.menuFileLoadFileActrion = QAction('[PyQT DEMO] Otw. plik tekstowy', self)
		self.menuFileLoadFileActrion.triggered.connect(self.obslLoadFile)
		self.menuplik.addAction(self.menuFileLoadFileActrion) #laczy ladowanie pliku z menu plik w zakladce PyQt demo zbior f
		self.textBrowser_pb_2.clicked.connect(self.obslLoadFile2) #laczy nacisniecie 'LADUJ PLIK' (textBrowser_pb_2) z metoda 'obslLoadFile'

		self.menuHelpAbout = QAction('O autorach', self)
		self.menuHelpAbout.triggered.connect(self.obslMenuHelpAbout)
		self.menupomoc.addAction(self.menuHelpAbout)

		self.menuHelpAbout2 = QAction('O uczelni', self)
		self.menuHelpAbout2.triggered.connect(self.obslMenuHelpOUczelni)
		self.menupomoc.addAction(self.menuHelpAbout2)

		self.menuHelpAbout3 = QAction('O projekcie', self)
		self.menuHelpAbout3.triggered.connect(self.obslMenuHelpOProjekcie)
		self.menupomoc.addAction(self.menuHelpAbout3)

		self.startButton.clicked.connect(self.obslLoadAnalysis)

	#---konstruktor---#

	#---obsluga BUTTON 'AKT.CZAS' i Label---#
	def obslButtonLabel(self):

		self.label_lbl.setText(datetime.now().strftime("%H:%M:%S"))
		self.statusBar().showMessage('Aktualny czas: %s' % datetime.now().strftime("%H:%M:%S"))
	#---obsluga BUTTON 'AKT.CZAS' i Label---#

	#---obsluga BUTTON 'WPROWADZ' i LineEdit---#
	def obslButtonLineEdit(self):

		self.statusBar().showMessage('Wprowadzono w LineEdit: %s' % self.lineEdit_le.text())
	#---obsluga BUTTON 'WPROWADZ' i LineEdit---#

	#---obsluga RADIOBUTTON 'A' i 'B'---#
	def obslRadioButton(self, act_val):

		if act_val: #sprawdzamy czy sygnal od wybranego RadioButton
			self.statusBar().showMessage('Wybrano RadioButton: %s' % self.sender().text())
	#---obsluga RADIOBUTTON 'A' i 'B'---#

	#---obsluga COMBOBOX---#
	def obslComboBox(self, act_idx):

		self.statusBar().showMessage('Wybrano z ComboBox opcje #%s o wartosci: %s' % (act_idx, self.sender().currentText()))
	#---obsluga COMBOBOX---#

	#---obsluga SLIDER---#
	def obslSlider(self, act_val):

		self.slider_lbl.setText(str(act_val))
		self.statusBar().showMessage('Wartosc Slider: %s' % (act_val))
	#---obsluga SLIDER---#

	#---obsluga LOAD FILE---#
	def obslLoadFile(self):

		fname = QFileDialog.getOpenFileName(self, 'Open file', os.path.dirname(__file__), "Text files (*.txt)")

		if fname[0] != '':
			self.statusBar().showMessage('Otwieram plik: %s' % fname[0])
			fhook = open(fname[0], 'r')
			fcont = fhook.read()
			self.textBrowser_tbr.setText(fcont)
	#---obsluga LOAD FILE---#


	#---obsluga LOAD ANALYSIS---#
	def obslLoadAnalysis(self):

		global isEmpty
		if isEmpty != 0:	
			isEmpty = 0
			

			Analysis.min_date[0] = self.dateTimeEdit.date().year()
			Analysis.min_date[1] = self.dateTimeEdit.date().month()
			Analysis.min_date[2] = self.dateTimeEdit.date().day()
			
			Analysis.max_date[0] = self.dateTimeEdit_2.date().year()
			Analysis.max_date[1] = self.dateTimeEdit_2.date().month()
			Analysis.max_date[2] = self.dateTimeEdit_2.date().day()

			Analysis.min_time[0] = self.dateTimeEdit.time().hour()
			Analysis.min_time[1] = self.dateTimeEdit.time().minute()
			Analysis.min_time[2] = self.dateTimeEdit.time().second()

			Analysis.max_time[0] = self.dateTimeEdit_2.time().hour()
			Analysis.max_time[1] = self.dateTimeEdit_2.time().minute()
			Analysis.max_time[2] = self.dateTimeEdit_2.time().second()			

			Analysis.analysis_UI(Analysis.calendar, Analysis.timer, Analysis.acc_x, Analysis.acc_y, Analysis.acc_z, Analysis.counter)

			self.textBrowser_Analysis.setText('\n'.join(Analysis.analysis_table))
			self.lcdNumber.display(Analysis.analysis_lcd[0])
			self.lcdNumber_2.display(Analysis.analysis_lcd[1])

			Analysis.calendar = []
			Analysis.timer = []
			Analysis.acc_x = []
			Analysis.acc_y = []
			Analysis.acc_z = []
			Analysis.gyro_x = []
			Analysis.gyro_y = []
			Analysis.gyro_z = []
			Analysis.counter = []

			Analysis.min_date = []	
			Analysis.max_date = []
			Analysis.min_time = []
			Analysis.max_time = []
			
			
			
			
	#---obsluga LOAD ANALYSIS---#




	#---obsluga LOAD FILE2---#
	def obslLoadFile2(self):

		fname = QFileDialog.getOpenFileName(self, 'Open file', os.path.dirname(__file__), "Text files (*.txt)")

		if fname[0] != '' :
			global isEmpty
			isEmpty = 1
			self.statusBar().showMessage('Otwieram plik: %s' % fname[0])
			fhook = open(fname[0], 'r')
			fcont = fhook.read()
			Analysis.read_file(fname[0])
			
			Analysis.min_date = Analysis.calendar[0].split('.')	
			Analysis.max_date = Analysis.calendar[-1].split('.')
			Analysis.min_time = Analysis.timer[0].split('.')
			Analysis.max_time = Analysis.timer[-1].split('.')

			d = QDate(int(Analysis.min_date[2]),int(Analysis.min_date[1]), int(Analysis.min_date[0]))
			self.dateTimeEdit.setMinimumDate(d)
			self.dateTimeEdit_2.setMinimumDate(d)
			d = QDate(int(Analysis.max_date[2]),int(Analysis.max_date[1]), int(Analysis.max_date[0]))
			self.dateTimeEdit.setMaximumDate(d)
			self.dateTimeEdit_2.setMaximumDate(d)
			d = QTime(int(Analysis.min_time[0]),int(Analysis.min_time[1]), int(Analysis.min_time[2]))
			self.dateTimeEdit.setMinimumTime(d)
			self.dateTimeEdit.setTime(d)
			self.dateTimeEdit_2.setMinimumTime(d)
			d = QTime(int(Analysis.max_time[0]),int(Analysis.max_time[1]), int(Analysis.max_time[2]))
			self.dateTimeEdit.setMaximumTime(d)
			self.dateTimeEdit_2.setMaximumTime(d)
			self.dateTimeEdit_2.setTime(d)
			self.textBrowser_tbr_2.setText(fcont)
	
	
		
	#---obsluga LOAD FILE2---#

	#---obsluga menu plik / about---#
	def obslMenuHelpAbout(self):

		okienko = QMessageBox()
		okienko.setStyleSheet('QLabel{min-width: 250px;}');
		okienko.setWindowTitle('O autorach')
		okienko.setText('Autorzy:\n\nSylwia Bakalarz\nAlbert Bogdanovic\nMichal Bober\nAda Kawala\nSofiya Makarenka\nStudenci 3 roku elektroniki')
		okienko.exec()
	#---obsluga menu plik / about---#
	#---obsluga menu plik / about---#
	def obslMenuHelpOUczelni(self):

		okienko = QMessageBox()
		okienko.setStyleSheet('QLabel{min-width: 250px;}');
		okienko.setWindowTitle('O uczelni')
		okienko.setText('Politechnika Warszawska - Wydzial Elektroniki i Technik Informacyjnych\nProjekt zaliczeniowy z przedmiotu EAME')
		okienko.exec()
	#---obsluga menu plik / about---#

	#---obsluga menu plik / about---#
	def obslMenuHelpOProjekcie(self):

		okienko = QMessageBox()
		okienko.setStyleSheet('QLabel{min-width: 500px;}');
		okienko.setWindowTitle('O projekcie')
		okienko.setText('Urządzenie do analizy oddechu. Prowadzący mgr inż. Krzysztof Dygnarowicz.\nCelem pracy jest zbudowanie takiego urządzenia, które pozwoliłoby nam monitorować oddech badanego przez całą noc.\nJest to ważny element leczenia, ponieważ wiemy czy następuje progres, bądź regres.\nDaje nam to informację o tym, czy wdrożone leczenie jest poprawne i wystarczające.')
		okienko.exec()
	#---obsluga menu plik / about---#
	
#-----klasa GUI / glowne okno-----#


#-----"main"-----#
if __name__ == '__main__':

	app = QApplication(sys.argv)
	window = MainWindow()
	window.show()
	app.exec()
#-----"main"-----#
