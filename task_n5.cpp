#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleValidator>
#include <cmath>

class FlexibleRodSolver : public QWidget {
    Q_OBJECT
public:
    FlexibleRodSolver(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *problemLabel = new QLabel(
            "<h2 style='color: #2c3e50; margin-bottom: 10px;'>Гибкий стержень с грузом</h2>"
            "<div style='background-color: #f5f5f5; padding: 15px; border-radius: 5px; border: 1px solid #ddd; margin-bottom: 15px;'>"
            "<p style='margin-top: 0;'><b>Условие:</b> Груз массой <b>m</b> на стержне длины <b>L</b> вращается с ω₀. "
            "При увеличении скорости до <b>ω</b>, стержень отклоняется на угол <b>α</b>.</p>"
            "<p><b>Требуется найти:</b></p>"
            "<ol>"
            "<li>Угол отклонения <b>α</b></li>"
            "<li>Натяжение стержня <b>T</b></li>"
            "<li>Работу <b>A</b> для изменения скорости</li>"
            "</ol>"
            "</div>"
            );
        problemLabel->setWordWrap(true);

        mInput = createInputField("Масса груза m (кг):");
        LInput = createInputField("Длина стержня L (м):");
        w0Input = createInputField("Начальная скорость ω₀ (рад/с):");
        wInput = createInputField("Конечная скорость ω (рад/с):");

        QPushButton *calculateButton = new QPushButton("Рассчитать");
        calculateButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #3498db;"
            "  color: white;"
            "  border: none;"
            "  padding: 10px 20px;"
            "  border-radius: 5px;"
            "  font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2980b9;"
            "}"
            );
        connect(calculateButton, &QPushButton::clicked, this, &FlexibleRodSolver::calculate);

        QLabel *resultsHeader = new QLabel("<h3 style='color: #2c3e50; margin-top: 15px;'>Результаты:</h3>");
        alphaLabel = createResultLabel("Угол отклонения α:");
        TLabel = createResultLabel("Натяжение T:");
        ALabel = createResultLabel("Работа A:");

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);
        mainLayout->addWidget(problemLabel);

        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->setSpacing(10);
        inputLayout->addRow(mInput->placeholderText(), mInput);
        inputLayout->addRow(LInput->placeholderText(), LInput);
        inputLayout->addRow(w0Input->placeholderText(), w0Input);
        inputLayout->addRow(wInput->placeholderText(), wInput);

        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(calculateButton, 0, Qt::AlignCenter);
        mainLayout->addWidget(resultsHeader);
        mainLayout->addWidget(alphaLabel);
        mainLayout->addWidget(TLabel);
        mainLayout->addWidget(ALabel);
        mainLayout->addStretch();

        setLayout(mainLayout);
        setWindowTitle("Задача №5, МЭ ВСОШ");
        setMinimumSize(550, 600);
    }

private slots:
    void calculate() {
        try {
            double m = getPositiveValue(mInput, "m");
            double L = getPositiveValue(LInput, "L");
            double w0 = getPositiveValue(w0Input, "ω₀");
            double w = getPositiveValue(wInput, "ω");

            if (w <= w0) throw std::invalid_argument("ω должна быть > ω₀");

            double g = 9.81;
            double cos_alpha = g / (L * w * w);
            if (cos_alpha >= 1.0) throw std::invalid_argument("Стержень не отклоняется (ω слишком мала)");
            double alpha_rad = std::acos(cos_alpha);
            double alpha_deg = alpha_rad * 180.0 / M_PI;

            double T = m * L * w * w / std::sin(alpha_rad);

            double K0 = 0.5 * m * L * L * w0 * w0;
            double K = 0.5 * m * L * L * w * w * std::sin(alpha_rad) * std::sin(alpha_rad);
            double U = m * g * L * (1 - std::cos(alpha_rad));
            double A = (K + U) - K0;


            alphaLabel->setText(QString("Угол отклонения α: <b>%1°</b>").arg(alpha_deg, 0, 'f', 2));
            TLabel->setText(QString("Натяжение T: <b>%1 Н</b>").arg(T, 0, 'f', 3));
            ALabel->setText(QString("Работа A: <b>%1 Дж</b>").arg(A, 0, 'f', 3));

        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

private:
    QLineEdit *mInput, *LInput, *w0Input, *wInput;
    QLabel *alphaLabel, *TLabel, *ALabel;

    QLineEdit* createInputField(const QString& placeholder) {
        QLineEdit* input = new QLineEdit;
        input->setPlaceholderText(placeholder);
        input->setValidator(new QDoubleValidator(0.0, 1e6, 5, this));
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
            throw std::invalid_argument(QString("%1 должно быть положительным").arg(name).toStdString());
        }
        return value;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FlexibleRodSolver solver;
    solver.show();
    return app.exec();
}

#include "main.moc"
