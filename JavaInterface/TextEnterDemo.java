import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

// 1. Реализуем интерфейс ActionListener вместо DocumentListener
public class TextEnterDemo extends JFrame implements ActionListener {

    private JTextField txtCat;
    private JLabel txtResult;

    public TextEnterDemo() {
        super("Нажмите Enter для вывода");

        this.setLayout(new FlowLayout());
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(600, 400);

        txtCat = new JTextField(20);
        txtResult = new JLabel("Текст появится после Enter...");
        txtResult.setForeground(Color.BLUE);

        this.add(new JLabel("Введите текст:"));
        this.add(txtCat);
        this.add(txtResult);

        // 2. Регистрируем слушатель на нажатие Enter
        txtCat.addActionListener(this);

        this.setVisible(true);
    }

    // 3. Этот метод вызывается ТОЛЬКО при нажатии Enter
    @Override
    public void actionPerformed(ActionEvent e) {
        react();
    }

    private void react() {
        String text = txtCat.getText();
        if (!text.isEmpty()) {
            txtResult.setText(text);
            // Опционально: очистить поле ввода после нажатия
            // txtCat.setText("");
        } else {
            txtResult.setText("Поле пустое!");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new TextEnterDemo());
    }
}
