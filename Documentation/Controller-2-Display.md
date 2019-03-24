<table style="text-align: left; width: 80%; height: 40%; margin-left: auto; margin-right: auto; font-family: Calibri;" border="1" cellpadding="2" cellspacing="0">
  <tbody>
    <tr style="font-weight: bold;" align="center">
      <td colspan="12" rowspan="1" style="vertical-align: top;">Controller -&gt; Display<br>
      </td>
    </tr>
    <tr style="font-weight: bold;">
      <td style="vertical-align: top; text-align: center;">B0</td>
      <td style="vertical-align: top; text-align: center;">B1<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B2<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B3<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B4<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B5<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B6<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B7<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B8<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B9<br>
      </td>
    </tr>
    <tr style="font-weight: bold;">
      <td style="vertical-align: top; text-align: center;">3A<br>
      </td>
      <td style="vertical-align: top; text-align: center;">2C<br>
      </td>
      <td style="vertical-align: top; text-align: center;">00<br>
      </td>
      <td style="vertical-align: top; text-align: center;">60<br>
      </td>
      <td style="vertical-align: top; text-align: center;">EA<br>
      </td>
      <td style="vertical-align: top; text-align: center;">00<br>
      </td>
      <td style="vertical-align: top; text-align: center;">76<br>
      </td>
      <td style="vertical-align: top; text-align: center;">01<br>
      </td>
      <td style="vertical-align: top; text-align: center;">0D<br>
      </td>
      <td style="vertical-align: top; text-align: center;">0A<br>
      </td>
    </tr>
    <tr>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 3A</span><br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 2C</span><br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Motor Power<br>
      <br>
      </span>00 to 2A:<br>
current power going to the motor<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Wheel rotating time</span> <span style="font-weight: bold;">(low byte)</span><br>
      <br>
60: no rotation<br>
      <span style="color: rgb(20, 20, 20); font-family: Georgia,&quot;Times New Roman&quot;,Times,serif; font-size: 14.6667px; font-style: normal; font-weight: 400; letter-spacing: normal; orphans: 2; text-align: left; text-indent: 0px; text-transform: none; white-space: normal; widows: 2; word-spacing: 0px; background-color: rgb(255, 255, 252); display: inline ! important; float: none;">v=1325/b3 (27,5")</span><br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Wheel rotating time </span><span style="font-weight: bold;">(high byte)</span></td>
      <td style="vertical-align: top; text-align: center; font-weight: bold;"><span style="font-weight: bold;">Motor running<br>
      <br>
      </span><span style="font-weight: normal;">00: Idle</span><br style="font-weight: normal;">
      <span style="font-weight: normal;">16: running</span><br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Checksum<br>
      </span>&nbsp;(B1+B2+B3+B4+B5) mod 256</td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Wheel rotating<br>
      </span><br>
00: no rotation<br>
01: rotating 6Km/h<br>
+- 0,8Km/h<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 0D</span><br>
      <br>
(CR)<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 0A</span><br>
      <br>
(LF)<br>
      </td>
    </tr>
  </tbody>
</table>