#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleValidator>
#include <cmath>

class PendulumCollision : public QWidget {
    Q_OBJECT
public:
    PendulumCollision(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *problemLabel = new QLabel(
            "<div style='background-color: #f5f5f5; padding: 15px; border-radius: 5px; border: 1px solid #ddd; margin-bottom: 15px;'>"
            "<p style='margin-top: 0;'><b>Условие:</b> Два шара массами <b>m₁</b> и <b>m₂</b> подвешены на невесомых нитях длины <b>L</b>, закрепленных в одной точке. "
            "Шар <b>m₁</b> отклоняют на угол <b>θ</b> и отпускают без начальной скорости. Происходит абсолютно упругое центральное столкновение с шаром <b>m₂</b>. "
            "После столкновения шар <b>m₂</b> движется по окружности до момента провисания нити.</p>"
            "<p><b>Требуется найти:</b></p>"
            "<ol>"
            "<li>Скорость шара <b>m₂</b> сразу после столкновения (<b>v₂</b>)</li>"
            "<li>Угол <b>φ</b>, при котором нить шара <b>m₂</b> начинает провисать</li>"
            "<li>Максимальную высоту <b>h</b> подъема шара <b>m₁</b> после столкновения</li>"
            "</ol>"
            "</div>"
            );
        problemLabel->setWordWrap(true);

        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->setSpacing(10);

        m1Input = createInputField("Масса первого шара m₁ (кг):");
        m2Input = createInputField("Масса второго шара m₂ (кг):");
        LInput = createInputField("Длина нити L (м):");
        thetaInput = createInputField("Угол отклонения θ (градусы):");

        inputLayout->addRow(m1Input->placeholderText(), m1Input);
        inputLayout->addRow(m2Input->placeholderText(), m2Input);
        inputLayout->addRow(LInput->placeholderText(), LInput);
        inputLayout->addRow(thetaInput->placeholderText(), thetaInput);

        QPushButton *calculateButton = new QPushButton("Рассчитать");
        calculateButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #3498db;"
            "  color: white;"
            "  border: none;"
            "  padding: 10px 20px;"
            "  border-radius: 5px;"
            "  font-size: 14px;"
            "  min-width: 100px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2980b9;"
            "}"
            );
        connect(calculateButton, &QPushButton::clicked, this, &PendulumCollision::calculate);

        QLabel *resultsHeader = new QLabel("<h3 style='color: #2c3e50; margin-top: 15px;'>Результаты расчета:</h3>");

        v2Label = createResultLabel("Скорость v₂:");
        phiLabel = createResultLabel("Угол провисания φ:");
        hLabel = createResultLabel("Высота подъема h:");

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);
        mainLayout->addWidget(problemLabel);
        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(calculateButton, 0, Qt::AlignCenter);
        mainLayout->addWidget(resultsHeader);
        mainLayout->addWidget(v2Label);
        mainLayout->addWidget(phiLabel);
        mainLayout->addWidget(hLabel);
        mainLayout->addStretch();

        setLayout(mainLayout);
        setWindowTitle("Задача №3, МЭ ВСОШ");
        setMinimumSize(550, 600);
    }

private slots:
    void calculate() {
        try {
            double m1 = getPositiveValue(m1Input, "масса m₁");
            double m2 = getPositiveValue(m2Input, "масса m₂");
            double L = getPositiveValue(LInput, "длина нити L");
            double theta_deg = getAngleValue(thetaInput);

            double theta = theta_deg * M_PI / 180.0;
            double v1 = sqrt(2 * 9.81 * L * (1 - cos(theta)));
            double v2 = (2 * m1) / (m1 + m2) * v1;

            double cos_phi = v2*v2 / (9.81 * L);
            if (cos_phi >= 1.0) throw std::runtime_error("Нить не провиснет при данных параметрах");
            double phi = acos(cos_phi) * 180.0 / M_PI;

            double u1 = (m1 - m2)/(m1 + m2) * v1;
            double h = u1*u1 / (2 * 9.81);

            v2Label->setText(QString("Скорость v₂: <b>%1 м/с</b>").arg(v2, 0, 'f', 3));
            phiLabel->setText(QString("Угол провисания φ: <b>%1°</b>").arg(phi, 0, 'f', 2));
            hLabel->setText(QString("Высота подъема h: <b>%1 м</b>").arg(h, 0, 'f', 3));

        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка расчета", e.what());
        }
    }

private:
    QLineEdit *m1Input, *m2Input, *LInput, *thetaInput;
    QLabel *v2Label, *phiLabel, *hLabel;

    QLineEdit* createInputField(const QString& placeholder) {
        QLineEdit* input = new QLineEdit;
        input->setPlaceholderText(placeholder);
        input->setValidator(new QDoubleValidator(0.0, 1000.0, 5, this));
        input->setStyleSheet(
            "QLineEdit {"
            "  padding: 8px;"
            "  border: 1px solid #ccc;"
            "  border-radius: 4px;"
            "  min-width: 200px;"
            "}"
            );
        return input;
    }

    QLabel* createResultLabel(const QString& text) {
        QLabel* label = new QLabel(text);
        label->setStyleSheet(
            "QLabel {"
            "  margin: 8px 0;"
            "  padding: 5px;"
            "  font-size: 14px;"
            "}"
            );
        return label;
    }

    double getPositiveValue(QLineEdit* input, const QString& name) {
        bool ok;
        double value = input->text().toDouble(&ok);
        if (!ok || value <= 0) {
            throw std::invalid_argument(QString("Некорректное значение для %1").arg(name).toStdString());
        }
        return value;
    }

    double getAngleValue(QLineEdit* input) {
        bool ok;
        double angle = input->text().toDouble(&ok);
        if (!ok || angle <= 0 || angle >= 90) {
            throw std::invalid_argument("Угол θ должен быть в диапазоне: 0° < θ < 90°");
        }
        return angle;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PendulumCollision solver;
    solver.show();

    return app.exec();
}

#include "main.moc"
