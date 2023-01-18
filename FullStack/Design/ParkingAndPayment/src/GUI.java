import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class GUI extends JFrame {
    private int width = 10;
    private int height = 20;
    JFrame jFrame = new JFrame();
    JLabel jLabelPark = new JLabel("park Button");
    JLabel jLabelUn = new JLabel("Un park Button");
    JButton  jButton = new JButton("Button 1");
    JTextField jTextField = new JTextField(10);
    ParkingLotManager ParkingLotManager = new ParkingLotManager(10,10);

    public GUI() throws HeadlessException {
        System.out.println("start");
        setUp();
        System.out.println("configed");
    }

    private void setUp(){
        jFrame.setSize(300,300);
        jFrame.setTitle("Parking Lot");
        Container cp = jFrame.getContentPane();
        FlowLayout flow = new FlowLayout();
        cp.setLayout(flow);
        cp.add(jLabelPark);
        jFrame.add(jButton);
        cp.add(jTextField);
        ActionListener buttonListner = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("clicked");
                ParkingLotManager.parkVehicleUI();
                String text = jTextField.getText();
                String[] arr = text.split(",");
                System.out.println(text);
                Reservetion res = ParkingLotManager.parkVehicle(new Vehicle(Integer.parseInt(arr[0]), arr[1]));
                ParkingLotManager.showRecite(res);
            }
        };
        jButton.addActionListener(buttonListner);
        jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        jFrame.setVisible(true);
    }

    public static void main(String[] args) {
        GUI gui = new GUI();
    }
}
