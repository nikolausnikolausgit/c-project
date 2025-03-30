#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <cmath>

class PhysicsSolver : public QWidget {
    Q_OBJECT
public:
    PhysicsSolver(QWidget *parent = nullptr) : QWidget(parent) {

        QLabel *titleLabel = new QLabel("Решение задачи на закон сохранения импульса");
        QLabel *descriptionLabel = new QLabel("Введите входные данные:");

        MInput = new QLineEdit;
        mInput = new QLineEdit;
        lInput = new QLineEdit;
        alphaInput = new QLineEdit;

        QPushButton *calculateButton = new QPushButton("Рассчитать");
        connect(calculateButton, &QPushButton::clicked, this, &PhysicsSolver::calculate);

        velocityLabel = new QLabel("Скорость тележки (V): ");
        ratioLabel = new QLabel("Соотношение масс для остановки (m/M): ");

        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow("Масса тележки (M):", MInput);
        formLayout->addRow("Масса шарика (m):", mInput);
        formLayout->addRow("Длина стержня (l):", lInput);
        formLayout->addRow("Угол отклонения (α, градусы):", alphaInput);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(titleLabel);
        mainLayout->addWidget(descriptionLabel);
        mainLayout->addLayout(formLayout);
        mainLayout->addWidget(calculateButton);
        mainLayout->addWidget(velocityLabel);
        mainLayout->addWidget(ratioLabel);

        setLayout(mainLayout);
    }

private slots:
    void calculate() {
        try {
            double M = MInput->text().toDouble();
            double m = mInput->text().toDouble();
            double l = lInput->text().toDouble();
            double alpha_deg = alphaInput->text().toDouble();

            if (M <= 0 || m <= 0 || l <= 0 || alpha_deg <= 0) {
                throw std::invalid_argument("Все величины должны быть положительными");
            }

            double alpha_rad = alpha_deg * M_PI / 180.0;

            double V = sqrt((2 * m * m * 9.81 * l * (1 - cos(alpha_rad))) / (M * (M + m)));

            double ratio = (V < 1e-6) ? 0 : m / M;

            velocityLabel->setText(QString("Скорость тележки (V): %1 м/с").arg(V, 0, 'f', 4));
            ratioLabel->setText(QString("Соотношение масс для остановки (m/M): %1").arg(ratio, 0, 'f', 2));

        } catch (...) {
            QMessageBox::warning(this, "Ошибка", "Проверьте правильность введенных данных");
        }
    }

private:
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
    solver.setWindowTitle("Задача №1, МЭ ВСОШ");
    solver.resize(400, 300);
    solver.show();

    return app.exec();
}

#include "main.moc"
