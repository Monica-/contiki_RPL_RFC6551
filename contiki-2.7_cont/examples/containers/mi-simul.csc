<?xml version="1.0" encoding="UTF-8"?>
<simconf>
  <project EXPORT="discard">[APPS_DIR]/mrm</project>
  <project EXPORT="discard">[APPS_DIR]/mspsim</project>
  <project EXPORT="discard">[APPS_DIR]/avrora</project>
  <project EXPORT="discard">[APPS_DIR]/serial_socket</project>
  <project EXPORT="discard">[APPS_DIR]/collect-view</project>
  <project EXPORT="discard">[APPS_DIR]/powertracker</project>
  <simulation>
    <title>My simulation</title>
    <randomseed>123456</randomseed>
    <motedelay_us>100000</motedelay_us>
    <radiomedium>
      se.sics.cooja.radiomediums.UDGM
      <transmitting_range>20.0</transmitting_range>
      <interference_range>25.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>40000</logoutput>
    </events>
    <motetype>
      se.sics.cooja.mspmote.Z1MoteType
      <identifier>z11</identifier>
      <description>server</description>
      <firmware EXPORT="copy">[CONFIG_DIR]/server.z1</firmware>
      <moteinterface>se.sics.cooja.interfaces.Position</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspButton</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspDefaultSerial</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <motetype>
      se.sics.cooja.mspmote.Z1MoteType
      <identifier>z12</identifier>
      <description>client</description>
      <firmware EXPORT="copy">[CONFIG_DIR]/client.z1</firmware>
      <moteinterface>se.sics.cooja.interfaces.Position</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>se.sics.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspButton</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspDefaultSerial</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspLED</moteinterface>
      <moteinterface>se.sics.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>54.335467383479994</x>
        <y>4.747565694681829</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>1</id>
      </interface_config>
      <motetype_identifier>z11</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>63.06567445836441</x>
        <y>17.713731231936247</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>2</id>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspDefaultSerial
        <history>net~;</history>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints>
        <breakpoint>
          <stops>false</stops>
          <codefile>[CONFIG_DIR]/../../core/net/rpl/rpl-mrhof.c</codefile>
          <line>168</line>
          <contikicode>printf("Rank: %u, Rank pad: %u, link met: %u\n", new_rank,p-&gt;rank, p-&gt;link_metric/ RPL_DAG_MC_ETX_DIVISOR);</contikicode>
          <color>-16777216</color>
        </breakpoint>
        <breakpoint>
          <stops>false</stops>
          <codefile>[CONFIG_DIR]/../../core/net/rpl/rpl-mrhof.c</codefile>
          <line>146</line>
          <contikicode>if(p == NULL) {</contikicode>
          <color>-16777216</color>
        </breakpoint>
      </breakpoints>
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>53.40562284295976</x>
        <y>19.00518198265879</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>3</id>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>43.43562304738165</x>
        <y>15.905700180924677</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>4</id>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>42.14417229665909</x>
        <y>29.595078138583713</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>5</id>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>58.933032056052255</x>
        <y>31.919689489884306</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>6</id>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>47.05168514940479</x>
        <y>40.95984474494215</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>7</id>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        se.sics.cooja.interfaces.Position
        <x>53.50893890301754</x>
        <y>40.70155459479764</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspMoteID
        <id>8</id>
      </interface_config>
      <interface_config>
        se.sics.cooja.mspmote.interfaces.MspDefaultSerial
        <history>net~;</history>
      </interface_config>
      <motetype_identifier>z12</motetype_identifier>
    </mote>
  </simulation>
  <plugin>
    se.sics.cooja.plugins.SimControl
    <width>280</width>
    <z>0</z>
    <height>160</height>
    <location_x>191</location_x>
    <location_y>3</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>se.sics.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <skin>se.sics.cooja.plugins.skins.LEDVisualizerSkin</skin>
      <viewport>3.87161492391604 0.0 0.0 3.87161492391604 -131.16600641983428 -3.580746195802032</viewport>
    </plugin_config>
    <width>154</width>
    <z>3</z>
    <height>236</height>
    <location_x>6</location_x>
    <location_y>-1</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.RadioLogger
    <plugin_config>
      <split>150</split>
      <formatted_time />
      <showdups>false</showdups>
      <hidenodests>false</hidenodests>
      <analyzers name="6lowpan" />
      <alias payload=" 97: 15.4 D C10C:0000:0000:0001 FFFF|IPHC|ICMPv6 RPL DIO|AAAA0000 00000000 00000000 00000001 040E0008 0C0A0700 01000001 00FFFFFF 081E4040 00000000 00000000 00000000 AAAA0000 00000000 00000000 00000000" alias="DIO1" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0002 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0003 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0008 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0007 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload="  5: 15.4 A |020002AA 96" alias="ACK" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0006 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0005 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
      <alias payload=" 64: 15.4 D C10C:0000:0000:0004 FFFF|IPv6|ICMPv6 RPL DIS|0000" alias="DIS" />
    </plugin_config>
    <width>803</width>
    <z>2</z>
    <height>424</height>
    <location_x>371</location_x>
    <location_y>166</location_y>
  </plugin>
  <plugin>
    se.sics.cooja.plugins.LogListener
    <plugin_config>
      <filter />
      <formatted_time />
      <coloring />
    </plugin_config>
    <width>1220</width>
    <z>1</z>
    <height>459</height>
    <location_x>4</location_x>
    <location_y>386</location_y>
  </plugin>
</simconf>

