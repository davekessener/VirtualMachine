package de.hawhh.informatik.sml.kino.werkzeuge.langchooser;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import de.hawhh.informatik.sml.kino.materialien.StringLiterals;

public class LanguageChooserUI
{
    private JDialog frame_;
    private JLabel query_;
    private JButton ok_;
    private JComboBox<String> choices_;
    
    public static abstract class Listener implements ActionListener
    {
        @Override
        public void actionPerformed(ActionEvent e) { act(); }
        public abstract void act();
    }
    
    public static abstract class ComboListener implements ActionListener
    {
        @SuppressWarnings("unchecked")
        @Override
        public void actionPerformed(ActionEvent e) { act(((JComboBox<String>) e.getSource()).getSelectedIndex()); }
        public abstract void act(int idx);
    }
    
    public LanguageChooserUI()
    {
        frame_ = createFrame();
    }

    public void show()
    {
        frame_.pack();
        frame_.setLocationRelativeTo(null);
        frame_.setVisible(true);
    }
    
    public void registerOKListener(Listener l)
    {
        ok_.addActionListener(l);
    }
    
    public void registerComboListener(ComboListener l)
    {
        choices_.addActionListener(l);
    }
    
    private JDialog createFrame()
    {
        JDialog f = new JDialog(null, "", Dialog.ModalityType.APPLICATION_MODAL);
        f.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        f.getContentPane().setLayout(new BorderLayout());
        
        populatePane(f.getContentPane());
        
        return f;
    }
    
    private void populatePane(Container content)
    {
        Dimension buf = new Dimension(0, 10);
        
        JPanel c = new JPanel();
        
        c.setLayout(new BoxLayout(c, BoxLayout.Y_AXIS));

        c.add(query_ = new JLabel());
        c.add(Box.createRigidArea(buf));
        c.add(choices_ = new JComboBox<String>());
        c.add(Box.createRigidArea(buf));
        
        JPanel btns = new JPanel();
        btns.setLayout(new BoxLayout(btns, BoxLayout.X_AXIS));
        btns.add(Box.createHorizontalGlue());
        btns.add(ok_ = new JButton());
        btns.add(Box.createRigidArea(buf));
        
        c.add(btns);
        
        c.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        content.add(c, BorderLayout.CENTER);
    }
    
    public void updateChoices(String[] s)
    {
        int idx = choices_.getSelectedIndex();
        choices_.setModel(new DefaultComboBoxModel<String>(s));
        if(idx >= 0 && idx < s.length) choices_.setSelectedIndex(idx);
    }
    
    public void update()
    {
        query_.setText(StringLiterals.Get(StringLiterals.LANGCHOICE_QUERY));
        ok_.setText(StringLiterals.Get(StringLiterals.OK));
        frame_.pack();
    }
    
    public void close()
    {
        frame_.dispose();
    }
}
