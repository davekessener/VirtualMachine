import java.sql.SQLException;

import viewer.service.Logger;
import viewer.service.connection.ConnectionService;
import viewer.tools.viewer.Viewer;
import javafx.application.Application;
import javafx.stage.Stage;


public class Main extends Application
{
    private ConnectionService service_;
    
    public static void main(String[] args) throws SQLException
    {
        launch(args);
    }

    @Override
    public void start(Stage primary) throws Exception
    {
        Logger.Log("Starting program.");
        
        (new Viewer(primary, service_ = new ConnectionService())).run();
    }
    
    @Override
    public void stop() throws Exception
    {
        service_.close();
        super.stop();
    }
}
