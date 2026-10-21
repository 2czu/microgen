import json
import sys
from pathlib import Path

import numpy as np
from PySide6.QtCore import Qt, QProcess
from PySide6.QtGui import QImage, QPixmap
from PySide6.QtWidgets import (
    QApplication, QDoubleSpinBox, QFormLayout, QLabel, QMainWindow,
    QPlainTextEdit, QPushButton, QSlider, QSpinBox, QVBoxLayout, QWidget,
)

MICROGEN = "build/src/microgen"
MICROGEN_DIR = Path(MICROGEN)
OUT_DIR = Path("out")
BIN_NAME = "vol.bin"


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("microgen")
        self.volume = None          # numpy array (x, y,z)
        self.proc = None

        # attributes

        self.size_in = QSpinBox()
        self.size_in.setRange(8, 256)
        self.size_in.setValue(64)

        self.n_in = QSpinBox()
        self.n_in.setRange(0, 5000)
        self.n_in.setValue(50)

        self.radius_in = QDoubleSpinBox()
        self.radius_in.setRange(0.5, 100.0)
        self.radius_in.setValue(17.0)

        self.seed_in = QSpinBox()
        self.seed_in.setRange(0, 2_000_000_000)
        self.seed_in.setValue(42)

        self.btn = QPushButton("Generate")
        self.btn.clicked.connect(self.generate)


        self.image_label = QLabel("No volume")
        self.image_label.setFixedSize(512, 512)
        self.image_label.setAlignment(Qt.AlignCenter)

        self.slider = QSlider(Qt.Horizontal)
        self.slider.setEnabled(False)
        self.slider.valueChanged.connect(self.show_slice)

        self.info = QLabel("")
        self.log = QPlainTextEdit()
        self.log.setReadOnly(True)
        self.log.setMaximumHeight(100)

        # layout
        
        form = QFormLayout()
        form.addRow("Size (voxels/side)", self.size_in)
        form.addRow("Spheres amount", self.n_in)
        form.addRow("Spheres radius", self.radius_in)
        form.addRow("Seed", self.seed_in)

        layout = QVBoxLayout()
        layout.addLayout(form)
        layout.addWidget(self.btn)
        layout.addWidget(self.image_label)
        layout.addWidget(self.slider)
        layout.addWidget(self.info)
        layout.addWidget(self.log)

        central = QWidget()
        central.setLayout(layout)
        self.setCentralWidget(central)

    # start the microgen program with QProcess
    def generate(self):
        OUT_DIR.mkdir(parents=True, exist_ok=True)
        if not MICROGEN_DIR.exists():
            self.log.appendPlainText("The build folder doesn't exist yet, do: cmake -B build && cmake --build build")
            return
        args = [
            "--size", str(self.size_in.value()),
            "--n", str(self.n_in.value()),
            "--radius", str(self.radius_in.value()),
            "--seed", str(self.seed_in.value()),
            "--outfile", str(BIN_NAME),
            "--outdir", str(OUT_DIR),
        ]
        self.btn.setEnabled(False)
        self.log.appendPlainText("Start : microgen " + " ".join(args))

        self.proc = QProcess(self)
        self.proc.readyReadStandardError.connect(
            lambda: self.log.appendPlainText(
                bytes(self.proc.readAllStandardError()).decode(errors="replace")))
        self.proc.finished.connect(self.on_finished)
        self.proc.start(MICROGEN, args)

    # results loading
    def on_finished(self, exit_code, exit_status):
        self.btn.setEnabled(True)
        if exit_code != 0:
            self.log.appendPlainText(f"Failure (code {exit_code})")
            return

        meta = json.loads((OUT_DIR / "meta.json").read_text())
        n = meta["size"]
        self.volume = np.fromfile(OUT_DIR / BIN_NAME, dtype=np.uint8).reshape((n, n, n))

        self.slider.setEnabled(True)
        self.slider.setRange(0, n - 1)
        self.slider.setValue(n // 2)
        self.info.setText(f"Porosity : {meta['porosity']:.4f}")
        self.show_slice(self.slider.value())

    # show every slice depending on a slider
    def show_slice(self, k):
        if self.volume is None:
            return
        sl = np.ascontiguousarray(self.volume[k] * 255, dtype=np.uint8)
        h, w = sl.shape
        img = QImage(sl.data, w, h, w, QImage.Format_Grayscale8).copy()
        pix = QPixmap.fromImage(img).scaled(
            512, 512, Qt.KeepAspectRatio, Qt.FastTransformation)
        self.image_label.setPixmap(pix)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec())