#!/usr/bin/python3
from scope import Scope,hexAscii
from Trace import TraceSet,Trace 
import sys
    
from PyQt5.QtChart import QChart, QChartView, QLineSeries
from PyQt5.QtGui import QPolygonF, QPainter
from PyQt5.QtWidgets import QMainWindow, QHBoxLayout, QVBoxLayout, QPushButton,QWidget,QLabel,QComboBox, QApplication, QCheckBox, QLineEdit
from PyQt5.QtCore import pyqtSlot,Qt,QTimer
import numpy as np

class LabeledComboBox(QComboBox):
    def __init__(self,label='Label',parent=None,items=[],itemLabels=None):
        super(LabeledComboBox, self).__init__(parent=parent)

        layout = QHBoxLayout()
        self.setLayout(layout)
        layout.addWidget(QLabel(label))

        layout.margin =0
        layout.setContentsMargins(0,0,0,0)
    
        self.comboBox = QComboBox()
        layout.addWidget(self.comboBox)
        
        self.hasLabels = False
        if not itemLabels:
            self.comboBox.addItems(items)
        else:
            for item in zip(itemLabels,items):
                self.comboBox.addItem(item[0],item[1])
                self.hasLabels = True

    def getInt(self):
        if self.hasLabels:
            return int(str(self.comboBox.currentData()))
        return int(str(self.comboBox.currentText()))

def series_to_polyline(xdata, ydata):
    """Convert series data to QPolygon(F) polyline
    
    This code is derived from PythonQwt's function named 
    `qwt.plot_curve.series_to_polyline`"""
    size = len(xdata)
    polyline = QPolygonF(size)
    pointer = polyline.data()
    dtype, tinfo = np.float, np.finfo  # integers: = np.int, np.iinfo
    pointer.setsize(2*polyline.size()*tinfo(dtype).dtype.itemsize)
    memory = np.frombuffer(pointer, dtype)
    memory[:(size-1)*2+1:2] = xdata
    memory[1:(size-1)*2+2:2] = ydata
    return polyline    
    
class TestWindow(QMainWindow):
    def __init__(self,scope,parent=None):
        super(TestWindow, self).__init__(parent=parent)

        self.scope = scope
        self.ncurves = 0
        self.chart = QChart()
        self.chart.legend().hide()
        self.view = QChartView(self.chart)
        self.view.setRenderHint(QPainter.Antialiasing)

        controlLayout = QHBoxLayout()
        controlLayout.setContentsMargins(0,0,0,0)
        controlWidget = QWidget()
        controlWidget.setLayout(controlLayout)

        self.runButton = QPushButton("Get")
        controlLayout.addWidget(self.runButton)

	# Auto refresh timer and control (For a demo)
        self.auto_box = QCheckBox("Auto (vroom vroom)")
        controlLayout.addWidget(self.auto_box)

        self._refreshTimer = QTimer()
        self._refreshTimer.timeout.connect(self.auto)
        self._refreshTimer.start(100.0) # timeout in milli seconds

        self.channel1 = QCheckBox("fetch 12bit")
        controlLayout.addWidget(self.channel1)

        controlLayout.addWidget(QLabel("Delay"))
        self.delay = QLineEdit("0")
        controlLayout.addWidget(self.delay)
        controlLayout.addWidget(QLabel("NumSamples"))
        self.numSamples = QLineEdit("3000")
        controlLayout.addWidget(self.numSamples)

        self.speedsComboBox = LabeledComboBox(label='Timebase',items=[ str(x) for x in list(range(8))] )
        controlLayout.addWidget(self.speedsComboBox)

        self.layout = QVBoxLayout()
        self.layout.addWidget(controlWidget)
        self.layout.addWidget(self.view)

        self.layoutWidget = QWidget()
        self.layoutWidget.setLayout(self.layout)

        self.runButton.clicked.connect(self.on_get)

        self.setCentralWidget(self.layoutWidget)

        self.show()

    @pyqtSlot()
    def auto(self):
        if self.auto_box.isChecked():
            self.on_get()

    @pyqtSlot()
    def on_get(self):
        self.chart.removeAllSeries()

        scope.setSampleSpeed( self.speedsComboBox.getInt() )
        scope.set12Bits(self.channel1.isChecked())
        scope.setDelay(int(str(self.delay.displayText())))


        # There is some weird behaviour in 12 bit mode
        numSamples = int(str(self.numSamples.displayText()))
        scope.setSamples(min(7*1024+512,numSamples))

        ch1data = scope_getSamples(self.scope)

        self.add_data(range(len(ch1data)),ch1data, color=Qt.red)

        self.set_title("Semi live Scope")
        
    def set_title(self, title):
        self.chart.setTitle(title)

    def add_data(self, xdata, ydata,color):
        curve = QLineSeries()
        pen = curve.pen()
        pen.setColor(color)
        pen.setWidthF(.1)
        #curve.setPen(pen)
        curve.setUseOpenGL(True)
        curve.append(series_to_polyline(xdata, ydata))
        self.chart.addSeries(curve)
        self.chart.createDefaultAxes()
        self.ncurves += 1

def scope_getSamples(scope):
    scope.arm()
    if not scope.waitForCompletion():
        print("Missed Trigger")
    return scope.getSamples()

if __name__ == '__main__':
    app = QApplication(sys.argv)

    scope = Scope('/dev/ttyACM0',
            timeout=1.5,
            sampleSpeed=2,
            bits12=True,
            bias=0x444,
            gain= 0,
            delay = 1,
            numSamples=1500
            )
    window = TestWindow(scope)

    npoints = 1000000
    window.setWindowTitle("Simple performance example")

    window.show()
    window.resize(500, 400)

    sys.exit(app.exec_())
