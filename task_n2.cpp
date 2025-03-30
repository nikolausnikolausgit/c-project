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

class PhysicsSolver : public QWidget {
    Q_OBJECT
public:
    PhysicsSolver(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *problemLabel = new QLabel(
            "<h2 style='color: #2c3e50;'>Закон сохранения импульса</h2>"
            "<div style='background-color: #f8f9fa; padding: 10px; border-radius: 5px;'>"
            "<p><b>Условие задачи:</b></p>"
            "<p>Брусок массой <b>M</b> покоится на столе. Пуля массой <b>m</b>, "
            "летящая со скоростью <b>v₀</b>, застревает в нём. Коэффициент трения между бруском и столом <b>μ</b>.</p>"
            "<p><b>Требуется:</b></p>"
            "<ol>"
            "<li>Найти скорость системы после удара (<b>u</b>)</li>"
            "<li>Определить путь до остановки (<b>S</b>)</li>"
            "</ol>"
            "</div>"
            );
        problemLabel->setWordWrap(true);

        MInput = createInputField("Масса бруска (M), кг:");
        mInput = createInputField("Масса пули (m), кг:");
        v0Input = createInputField("Скорость пули (v₀), м/с:");
        muInput = createInputField("Коэф. трения (μ):");

        QPushButton *calculateButton = new QPushButton("Рассчитать");
        calculateButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #3498db;"
            "  color: white;"
            "  border: none;"
            "  padding: 8px;"
            "  border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2980b9;"
            "}"
            );
        connect(calculateButton, &QPushButton::clicked, this, &PhysicsSolver::calculate);

        velocityLabel = new QLabel("Скорость после удара (u): ");
        distanceLabel = new QLabel("Путь до остановки (S): ");

        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->addRow("Масса бруска (M), кг:", MInput);
        inputLayout->addRow("Масса пули (m), кг:", mInput);
        inputLayout->addRow("Скорость пули (v₀), м/с:", v0Input);
        inputLayout->addRow("Коэф. трения (μ):", muInput);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(problemLabel);
        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(calculateButton);
        mainLayout->addWidget(velocityLabel);
        mainLayout->addWidget(distanceLabel);
        mainLayout->addStretch();

        setLayout(mainLayout);
        setWindowTitle("Задача №2, МЭ ВСОШ");
        setMinimumSize(500, 400);
    }

private slots:
    void calculate() {
        try {
            double M = MInput->text().toDouble();
            double m = mInput->text().toDouble();
            double v0 = v0Input->text().toDouble();
            double mu = muInput->text().toDouble();

            if (M <= 0 || m <= 0 || v0 <= 0 || mu <= 0) {
                throw std::invalid_argument("Все величины должны быть положительными");
            }

            double u = (m * v0) / (M + m);

            double S = (u * u) / (2 * mu * 9.81);

            velocityLabel->setText(QString("<b>Скорость после удара (u):</b> %1 м/с").arg(u, 0, 'f', 3));
            distanceLabel->setText(QString("<b>Путь до остановки (S):</b> %1 м").arg(S, 0, 'f', 3));

        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

private:
    QLineEdit *createInputField(const QString &placeholder) {
        QLineEdit *input = new QLineEdit;
        input->setPlaceholderText(placeholder);
        input->setValidator(new QDoubleValidator(0.0, 1000.0, 3, this));
        return input;
    }

    QLineEdit *MInput;
    QLineEdit *mInput;
    QLineEdit *v0Input;
    QLineEdit *muInput;
    QLabel *velocityLabel;
    QLabel *distanceLabel;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PhysicsSolver solver;
    solver.show();
    return app.exec();
}

#include "main.moc"
