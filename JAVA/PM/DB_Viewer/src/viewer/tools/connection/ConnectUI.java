package viewer.tools.connection;

import viewer.literals.language.Literals;
import viewer.literals.language.Strings;
import viewer.tools.ui.NumericField;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.RowConstraints;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;

class ConnectUI
{
    private Node pane_;
    private Button connect_, test_;
    private TextField name_, server_, sid_, user_, password_;
    private NumericField port_;
    private Label indicator_;
    private Button cancel_;
    private Node form_;
    
    public ConnectUI()
    {
        pane_ = createUI();
    }
    
    public Node getUI()
    {
        return pane_;
    }
    
    public String getName() { return name_.getText(); }
    public String getServer() { return server_.getText(); }
    public String getSID() { return sid_.getText(); }
    public int getPort() { return port_.getValue(); }
    public String getUser() { return user_.getText(); }
    public String getPassword() { return password_.getText(); }
    
    public void registerConnect(EventHandler<ActionEvent> h)
    {
        connect_.setOnAction(h);
    }
    
    public void registerTest(EventHandler<ActionEvent> h)
    {
        test_.setOnAction(h);
    }
    
    public void registerCancel(EventHandler<ActionEvent> h)
    {
        cancel_.setOnAction(h);
    }
    
    // # ---------------------------------------------------------------------
    
    public void enableUILater(boolean f) { Platform.runLater(() -> enableUI(f)); }
    public void enableUI(boolean f)
    {
        form_.setDisable(!f);
    }

    public void setInfoLater(String s) { Platform.runLater(() -> setInfo(s)); }
    public void setInfo(String s) { setInfo(s, Strings.C_DEFAULT); }
    public void setInfoLater(String s, String c) { Platform.runLater(() -> setInfo(s, c)); }
    public void setInfo(String s, String c)
    {
        indicator_.setText(Literals.Get(s));
        indicator_.setTextFill(Color.web(Literals.Get(c)));
    }
    
    // # ---------------------------------------------------------------------
    
    private Node createUI()
    {
        StackPane pane = new StackPane();
        Node indicator = createIndicator();
        form_ = createForm();
        
        pane.getChildren().addAll(indicator, form_);
        
        return pane;
    }
    
    private Node createIndicator()
    {
        HBox pane = new HBox();
        
        pane.setSpacing(10);
        pane.setPadding(new Insets(10, 10, 10, 10));
        pane.setAlignment(Pos.BOTTOM_LEFT);

        cancel_ = new Button("X");
        indicator_ = new Label();
        
//        pane.getChildren().add(cancel_);
        pane.getChildren().add(indicator_);
        
        return pane;
    }
    
    private Node createForm()
    {
        GridPane grid = new GridPane();
        
        configureGrid(grid);
        
        addLabel(grid, Strings.S_ESTABLISH_NAME, 0, 0);
        addLabel(grid, Strings.S_ESTABLISH_SERVER, 0, 1);
        addLabel(grid, Strings.S_ESTABLISH_USER, 0, 2);
        addLabel(grid, Strings.S_ESTABLISH_INSTANCE, 2, 0);
        addLabel(grid, Strings.S_ESTABLISH_PORT, 2, 1);

        grid.add(name_ = new TextField(), 1, 0);
        grid.add(sid_ = new TextField(), 3, 0);
        grid.add(server_ = new TextField(), 1, 1);
        grid.add(port_ = new NumericField(), 3, 1);

        sid_.setMinWidth(20);
        sid_.setPrefWidth(80);
        port_.setMinWidth(20);
        port_.setPrefWidth(80);
        
        grid.add(createUserPassword(), 1, 2, 3, 1);
        grid.add(createButtons(), 0, 3, 4, 1);
        
        return grid;
    }
    
    private Node createButtons()
    {
        HBox pane = new HBox();
        
        pane.setSpacing(10);
        pane.setAlignment(Pos.BOTTOM_RIGHT);

        pane.getChildren().add(test_ = new Button(Literals.Get(Strings.B_ESTABLISH_TEST)));
        pane.getChildren().add(connect_ = new Button(Literals.Get(Strings.B_ESTABLISH_CONNECT)));
        
        return pane;
    }
    
    private void addLabel(GridPane grid, String s, int x, int y) { addLabel(grid, s, x, y, 1, 1); }
    private void addLabel(GridPane grid, String s, int x, int y, int w, int h)
    {
        Label lbl = new Label(Literals.Get(s));
        GridPane.setHalignment(lbl, HPos.RIGHT);
        grid.add(lbl, x, y, w, h);
    }
    
    private Node createUserPassword()
    {
        GridPane pane = new GridPane();
        
        pane.setHgap(10);
        
        ColumnConstraints cc = new ColumnConstraints();
        cc.setHgrow(Priority.SOMETIMES);
        pane.getColumnConstraints().add(cc);
        pane.getColumnConstraints().add(new ColumnConstraints());
        pane.getColumnConstraints().add(cc);
        
        pane.add(user_ = new TextField(), 0, 0);
        addLabel(pane, Strings.S_ESTABLISH_PASSWORD, 1, 0);
        pane.add(password_ = new PasswordField(), 2, 0);
        
        return pane;
    }
    
    private void configureGrid(GridPane grid)
    {
        grid.setAlignment(Pos.TOP_CENTER);
        grid.setHgap(10.0);
        grid.setVgap(10.0);
        grid.setPadding(new Insets(15, 10, 10, 10));

        ColumnConstraints cc = new ColumnConstraints();
        cc.setHgrow(Priority.ALWAYS);
        grid.getColumnConstraints().add(new ColumnConstraints());
        grid.getColumnConstraints().add(cc);
        
        RowConstraints rc = new RowConstraints();
        rc.setVgrow(Priority.ALWAYS);
        grid.getRowConstraints().add(new RowConstraints());
        grid.getRowConstraints().add(new RowConstraints());
        grid.getRowConstraints().add(new RowConstraints());
        grid.getRowConstraints().add(rc);
    }
}
