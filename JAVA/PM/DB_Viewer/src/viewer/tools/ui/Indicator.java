package viewer.tools.ui;

import javafx.scene.control.Alert;

public interface Indicator
{
    void setColor(String s);
    void setInfo(String s);
    void setEnabled(boolean f);
    void alert(Alert.AlertType type, String title, String msg);
}
