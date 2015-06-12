import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.reflect.Type;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;
import java.util.Map;

import javax.swing.SwingUtilities;

import oracle.jdbc.pool.OracleDataSource;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import com.google.gson.reflect.TypeToken;

@SuppressWarnings("all")
public class Startup
{
    public static void main(String[] args) throws IOException, SQLException, ClassNotFoundException
    {
        Class.forName ("oracle.jdbc.driver.OracleDriver");
        /* Baue eine Verbindung auf */
        Connection con;
        String url = "jdbc:oracle:thin:@ora14.informatik.haw-hamburg.de:1521:inf14";
        con = DriverManager.getConnection(url, "abp403", "DaV718227");
        /* Erstelle die Anweisung und uebergebe an das DBS */
        Statement statement = con.createStatement ( );
        String anfrage = "INSERT INTO Kunde values (10, 'Posey', 'Tyler', 'Beacon Hills')";
        ResultSet ergebnis = statement.executeQuery(anfrage);

//        OracleDataSource ods = new OracleDataSource();
////      ods.setURL("jdbc:oracle:thin:hr/hr@localhost:1521/oracle");
//      ods.setURL("jdbc:oracle:thin:@ora14.informatik.haw-hamburg.de:1521:inf14");
//      ods.setUser("abp403");
//      ods.setPassword("DaV718227");
//      Connection conn = ods.getConnection();
//
//      // Create Oracle DatabaseMetaData object
//      DatabaseMetaData meta = conn.getMetaData();
//
//      // gets driver info:
//      System.out.println("JDBC driver version is " + meta.getDriverVersion());
//      
//      Statement s = conn.createStatement();
//      s.executeQuery("INSERT INTO Kunde values (10, 'Posey', 'Tyler', 'Beacon Hills')");
    }
    
    private static void test() throws IOException
    {
        File test = new File("./test.json");
        
//        testJACKSON(test);
        testGSON(test);
        
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run()
            {
                 createGUI();
            }
        });
    }

    private static void createGUI()
    {
        PaymentWerkzeug ui = new PaymentWerkzeug(900);
        ui.run();
    }
    
    private static class Config
    {
        public String total, cash, change;
        public Config(String total, String cash, String change)
        {
            this.total = total;
            this.cash = cash;
            this.change = change;
        }
    }
    
    private static class Wrapper
    {
        public Config EN, DE;
        public Wrapper(Config EN, Config DE) { this.EN = EN; this.DE = DE; }
    }
    
    private static void testGSON(File f) throws IOException
    {
        testWrite(f);
        testRead(f);
    }
    
    private static void testRead(File f) throws JsonSyntaxException, IOException
    {
        Gson gson = new Gson();
        BufferedReader in = new BufferedReader(new FileReader(f));
        Type mtype = new TypeToken<Map<String, String>>(){}.getType();
        Map<String, String> m = gson.fromJson(in.readLine(), mtype);
        System.out.println(m.get("b"));
    }
    
    private static void testWrite(File f) throws IOException
    {
        BufferedWriter w = new BufferedWriter(new FileWriter(f));
        Gson gson = new Gson();
        Config lang_EN = new Config("Total:", "Cash:", "Change:");
        Config lang_DE = new Config("Betrag:", "Bar:", "Zurueck:");
        
//        w.write(gson.toJson(new Wrapper(lang_EN, lang_DE)));
//        w.write("\n\n");
        
        Map<String, String> m = new HashMap<String, String>();
        m.put("a", "1");
        m.put("b", "dos");
        w.write(gson.toJson(m));
        w.close();
    }
    
//    private static void testJACKSON(File test)
//    {
//      JsonFactory json = new JsonFactory();
//
//      testGenerator(json, test);
//      testParser(json, test);
//    }
//
//    private static void testParser(JsonFactory json, File f) throws IOException
//    {
//        JsonParser parser = json.createParser(f);
//        parser.ge
//        
//        TreeNode root = parser.readValueAsTree();
//        TreeNode lang = root.get("lang");
//        TreeNode lang_DE = lang.get("DE");
//        System.out.println(lang_DE.get("total").asToken().asString());
//    }
//    
//    private static void testGenerator(JsonFactory json, File f) throws IOException
//    {
//        JsonGenerator gen = json.createGenerator(f, JsonEncoding.UTF8);
//        gen.useDefaultPrettyPrinter();
//        
//        gen.writeStartObject();
//            gen.writeObjectFieldStart("lang");
//                gen.writeObjectFieldStart("EN");
//                    gen.writeStringField("total", "Total:");
//                    gen.writeStringField("cash", "Cash:");
//                    gen.writeStringField("change", "Change:");
//                gen.writeEndObject();
//                gen.writeObjectFieldStart("DE");
//                    gen.writeStringField("total", "Betrag:");
//                    gen.writeStringField("cash", "Bar:");
//                    gen.writeStringField("change", "Zurueck:");
//                gen.writeEndObject();
//            gen.writeEndObject();
//        gen.writeEndObject();
//        
//        gen.flush();
//        gen.close();
//    }
}
