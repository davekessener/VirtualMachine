package de.hawhh.informatik.sml.kino.werkzeuge.newkasse;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class NewKasseUI
{
    private JFrame frame_;
    private JButton open_;
    
    public NewKasseUI()
    {
        frame_ = createUI();
    }
    
    public void show()
    {
        frame_.pack();
        frame_.setLocationRelativeTo(null);
        frame_.setVisible(true);
    }
    
    public void registerOnNewKasse(Listener l)
    {
        open_.addActionListener(l);
    }

    private JFrame createUI()
    {
        JFrame f = new JFrame("Kassen Manager");
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.getContentPane().setLayout(new BorderLayout());
        
        populatePane(f.getContentPane());
        
        return f;
    }
    
    private void populatePane(Container content)
    {
        JPanel c = new JPanel();
        
        c.setLayout(new BoxLayout(c, BoxLayout.Y_AXIS));

        c.add(open_ = new JButton("Oeffne neue Kasse"));
        
        c.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        content.add(c, BorderLayout.CENTER);
    }
    
    public static abstract class Listener implements ActionListener
    {
        @Override public void actionPerformed(ActionEvent e) { act(); }
        public abstract void act();
    }
}
