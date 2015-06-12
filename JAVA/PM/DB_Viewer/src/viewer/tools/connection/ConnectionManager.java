package viewer.tools.connection;

import viewer.exception.ConnectionFailureException;
import viewer.exception.URLException;
import viewer.literals.URL;
import viewer.literals.language.Literals;
import viewer.literals.language.Strings;
import viewer.service.connection.ConnectionService;
import javafx.scene.control.Alert;
import javafx.scene.control.Tab;

public class ConnectionManager
{
    private Tab tab_;
    private ConnectUI establishUI_;
    private ConnectionUI connectionUI_;
    private ConnectionService service_;

    public ConnectionManager(Tab tab, ConnectionService service)
    {
        tab_ = tab;
        establishUI_ = new ConnectUI();
        connectionUI_ = new ConnectionUI();
        service_ = service;

        tab_.setContent(establishUI_.getUI());

        registerHandlers();
    }

    private void test()
    {
        establishUI_.setInfo(Strings.S_ESTABLISH_TEST_INITIALIZATION);
        establishUI_.enableUI(false);
        service_.register(() -> doTestConnection());
    }

    private void connect()
    {
    }

    private void disconnect()
    {
    }

    private void registerHandlers()
    {
        establishUI_.registerTest(e -> { test(); });
        establishUI_.registerConnect(e -> { connect(); });
    }
    
    // # ---------------------------------------------------------------------------
    
    private void doTestConnection()
    {
        try
        {
            if(service_.doTestConnection(getURL(), establishUI_.getUser(), establishUI_.getPassword()))
            {
                establishUI_.setInfoLater(Strings.S_ESTABLISH_TEST_SUCCESS, Strings.C_SUCCESS);
            }
            else
            {
                establishUI_.setInfoLater(Strings.S_ESTABLISH_TEST_FAILURE, Strings.C_FAILURE);
            }
        }
        catch(ConnectionFailureException | URLException e)
        {
            establishUI_.setInfoLater(e.getMessage(), Strings.C_FAILURE);
        }

        establishUI_.enableUILater(true);
    }
    
    private URL getURL() throws URLException
    {
        return URL.Get(establishUI_.getServer(), establishUI_.getPort(), establishUI_.getSID());
    }

    private void displayAlert(Alert.AlertType type, String title, String msg) { displayAlert(type, title, null, msg); }
    private void displayAlert(Alert.AlertType type, String title, String header, String msg)
    {
        Alert alert = new Alert(type);
        alert.setTitle(Literals.Get(title));
        alert.setHeaderText(Literals.Get(header));
        alert.setContentText(Literals.Get(msg));
        alert.showAndWait();
    }
}
