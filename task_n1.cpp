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
        // Условие задачи с оформлением
        QLabel *problemLabel = new QLabel(
            "<h2 style='color: #2c3e50;'>Закон сохранения импульса</h2>"
            "<div style='background-color: #f8f9fa; padding: 10px; border-radius: 5px;'>"
            "<p><b>Условие задачи:</b></p>"
            "<p>На гладкой горизонтальной поверхности покоится тележка массой <b>M</b>. "
            "На тележке укреплён лёгкий жёсткий стержень длиной <b>l</b>, "
            "на конце которого закреплён маленький шарик массой <b>m</b>. "
            "Стержень отклоняют на угол <b>α</b> от вертикали и отпускают.</p>"
            "<p><b>Требуется:</b></p>"
            "<ol>"
            "<li>Найти скорость тележки <b>V</b> в момент прохождения вертикали</li>"
            "<li>Определить соотношение масс <b>m/M</b> для остановки тележки</li>"
            "</ol>"
            "</div>"
            );
        problemLabel->setWordWrap(true);

        // Создаем поля ввода
        MInput = createInputField("Масса тележки (M), кг:");
        mInput = createInputField("Масса шарика (m), кг:");
        lInput = createInputField("Длина стержня (l), м:");
        alphaInput = createInputField("Угол отклонения (α), градусы:");

        // Размещаем поля в форме
        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->addRow("Масса тележки (M), кг:", MInput);
        inputLayout->addRow("Масса шарика (m), кг:", mInput);
        inputLayout->addRow("Длина стержня (l), м:", lInput);
        inputLayout->addRow("Угол отклонения (α), градусы:", alphaInput);

        // Кнопка расчета
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

        // Поля вывода результатов
        velocityLabel = new QLabel("Скорость тележки (V): ");
        ratioLabel = new QLabel("Соотношение масс для остановки (m/M): ");

        // Основной layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(problemLabel);
        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(calculateButton);
        mainLayout->addWidget(velocityLabel);
        mainLayout->addWidget(ratioLabel);
        mainLayout->addStretch();

        setLayout(mainLayout);
        setWindowTitle("Задача №1, МЭ ВСОШ");
        setMinimumSize(500, 400);
    }

private slots:
    void calculate() {
        try {
            // Получаем значения из полей ввода
            double M = MInput->text().toDouble();
            double m = mInput->text().toDouble();
            double l = lInput->text().toDouble();
            double alpha_deg = alphaInput->text().toDouble();

            // Проверка ввода
            if (M <= 0 || m <= 0 || l <= 0 || alpha_deg <= 0) {
                throw std::invalid_argument("Все величины должны быть положительными");
            }

            // Переводим угол в радианы
            double alpha_rad = alpha_deg * M_PI / 180.0;

            // Расчет скорости тележки
            double numerator = 2 * m * m * 9.81 * l * (1 - cos(alpha_rad));
            double denominator = M * (M + m);
            double V = sqrt(numerator / denominator);

            // Расчет соотношения масс
            double ratio = (V < 1e-6) ? 0 : m / M;

            // Вывод результатов
            velocityLabel->setText(QString("<b>Скорость тележки (V):</b> %1 м/с").arg(V, 0, 'f', 4));
            ratioLabel->setText(QString("<b>Соотношение масс для остановки (m/M):</b> %1").arg(ratio, 0, 'f', 2));

        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

private:
    QLineEdit *createInputField(const QString &placeholder) {
        QLineEdit *input = new QLineEdit;
        input->setPlaceholderText(placeholder);
        input->setValidator(new QDoubleValidator(0.0, 1000.0, 2, this));
        return input;
    }

    QLineEdit *MInput;
    QLineEdit *mInput;
    QLineEdit *lInput;
    QLineEdit *alphaInput;
    QLabel *velocityLabel;
    QLabel *ratioLabel;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PhysicsSolver solver;
    solver.show();

    return app.exec();
}

#include "main.moc"
