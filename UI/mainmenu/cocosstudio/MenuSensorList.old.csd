<GameFile>
  <PropertyGroup Name="MenuSensorList" Type="Scene" ID="220c3fad-7e73-45f6-bd60-7ae64071f537" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Visible="False" ctype="GameNodeObjectData">
        <Size X="640.0000" Y="200.0000" />
        <Children>
          <AbstractNodeData Name="panelBackground" ActionTag="-2103491179" Tag="12" IconVisible="False" TopMargin="-936.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="640.0000" Y="1136.0000" />
            <Children>
              <AbstractNodeData Name="txtLastUpdatedAt" ActionTag="-908327902" Tag="9" IconVisible="False" LeftMargin="178.5000" RightMargin="178.5000" TopMargin="69.5000" BottomMargin="1043.5000" FontSize="20" LabelText="Last Update : 2015-01-20 22:40" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="283.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="320.0000" Y="1055.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="164" G="210" B="255" />
                <PrePosition X="0.5000" Y="0.9287" />
                <PreSize X="0.4422" Y="0.0202" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="listSensors" ActionTag="1799642126" Tag="41" IconVisible="False" TopMargin="216.0000" BottomMargin="100.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" IsBounceEnabled="True" ScrollDirectionType="0" DirectionType="Vertical" ctype="ListViewObjectData">
                <Size X="640.0000" Y="820.0000" />
                <AnchorPoint />
                <Position Y="100.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.0880" />
                <PreSize X="1.0000" Y="0.7218" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Image_1" ActionTag="-1573766456" Tag="14" IconVisible="False" TopMargin="99.0000" BottomMargin="1036.0000" Scale9Width="640" Scale9Height="1" ctype="ImageViewObjectData">
                <Size X="640.0000" Y="1.0000" />
                <AnchorPoint />
                <Position Y="1036.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.9120" />
                <PreSize X="1.0000" Y="0.0009" />
                <FileData Type="Normal" Path="split_hr.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="txtHeader" ActionTag="1868289395" Tag="16" IconVisible="False" LeftMargin="253.5000" RightMargin="253.5000" TopMargin="26.0000" BottomMargin="1026.0000" FontSize="36" LabelText="Sensors&#xA;" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="133.0000" Y="84.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="320.0000" Y="1068.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.9401" />
                <PreSize X="0.2078" Y="0.0739" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="imgSearch" ActionTag="263458143" Tag="13" IconVisible="False" LeftMargin="19.0000" RightMargin="571.0000" TopMargin="25.0000" BottomMargin="1061.0000" TouchEnable="True" Scale9Width="256" Scale9Height="256" ctype="ImageViewObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="44.0000" Y="1086.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0688" Y="0.9560" />
                <PreSize X="0.0781" Y="0.0440" />
                <FileData Type="Normal" Path="search_blue.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="imgSort" ActionTag="386669297" Tag="28" IconVisible="False" LeftMargin="95.0000" RightMargin="495.0000" TopMargin="25.0000" BottomMargin="1061.0000" TouchEnable="True" Scale9Width="256" Scale9Height="256" ctype="ImageViewObjectData">
                <Size X="50.0000" Y="50.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="120.0000" Y="1086.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1875" Y="0.9560" />
                <PreSize X="0.0781" Y="0.0440" />
                <FileData Type="Normal" Path="sort_blue.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="txtSortTypeHeader" ActionTag="941765573" Tag="47" IconVisible="False" LeftMargin="20.0000" RightMargin="475.0000" TopMargin="168.0000" BottomMargin="934.0000" FontSize="30" LabelText="Sort Type :" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="145.0000" Y="34.0000" />
                <AnchorPoint />
                <Position X="20.0000" Y="934.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="255" />
                <PrePosition X="0.0313" Y="0.8222" />
                <PreSize X="0.2266" Y="0.0299" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="txtSortType" ActionTag="-844109186" Tag="61" IconVisible="False" LeftMargin="168.0000" RightMargin="400.0000" TopMargin="168.0000" BottomMargin="934.0000" FontSize="30" LabelText="None" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="72.0000" Y="34.0000" />
                <AnchorPoint />
                <Position X="168.0000" Y="934.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="255" />
                <PrePosition X="0.2625" Y="0.8222" />
                <PreSize X="0.1125" Y="0.0299" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="panelDownHeader" ActionTag="-1567654712" Tag="62" IconVisible="False" TopMargin="156.0000" BottomMargin="920.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="640.0000" Y="60.0000" />
                <AnchorPoint />
                <Position Y="920.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="191" G="191" B="191" />
                <PrePosition Y="0.8099" />
                <PreSize X="1.0000" Y="0.0528" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panelWorld" ActionTag="1903500575" Tag="57" IconVisible="False" RightMargin="321.0000" TopMargin="98.0000" BottomMargin="978.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="2" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="319.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="txtWorld" ActionTag="-722739814" Tag="60" IconVisible="False" LeftMargin="127.5000" RightMargin="128.5000" TopMargin="16.0000" BottomMargin="16.0000" FontSize="24" LabelText="World" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="63.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="159.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="30" G="144" B="255" />
                    <PrePosition X="0.4984" Y="0.5000" />
                    <PreSize X="0.1975" Y="0.4667" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position Y="978.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition Y="0.8609" />
                <PreSize X="0.4984" Y="0.0528" />
                <SingleColor A="255" R="191" G="191" B="191" />
                <FirstColor A="255" R="191" G="191" B="191" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="panelFavorite" ActionTag="932994196" Tag="58" IconVisible="False" LeftMargin="321.0000" TopMargin="98.0000" BottomMargin="978.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="2" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="319.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="txtFavorite" ActionTag="342521966" Tag="61" IconVisible="False" LeftMargin="115.5000" RightMargin="116.5000" TopMargin="16.0000" BottomMargin="16.0000" FontSize="24" LabelText="Favorite" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="87.0000" Y="28.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="159.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="30" G="144" B="255" />
                    <PrePosition X="0.4984" Y="0.5000" />
                    <PreSize X="0.2727" Y="0.4667" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="321.0000" Y="978.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5016" Y="0.8609" />
                <PreSize X="0.4984" Y="0.0528" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="191" G="191" B="191" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="5.6800" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>