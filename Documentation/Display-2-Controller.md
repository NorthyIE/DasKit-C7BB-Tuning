<table style="text-align: left; width: 100%; height: 50%; margin-left: auto; margin-right: auto; font-family: Calibri;" border="1" cellpadding="2" cellspacing="0">
  <tbody>
    <tr style="font-weight: bold;" align="center">
      <td colspan="12" rowspan="1" style="vertical-align: top;">Display -&gt; Controller<br>
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
      <td style="vertical-align: top; text-align: center;">B10<br>
      </td>
      <td style="vertical-align: top; text-align: center;">B11<br>
      </td>
    </tr>
    <tr style="font-weight: bold;">
      <td style="vertical-align: top; text-align: center;">3A<br>
      </td>
      <td style="vertical-align: top; text-align: center;">2C<br>
      </td>
      <td style="vertical-align: top; text-align: center;">0A<br>
      </td>
      <td style="vertical-align: top; text-align: center;">46<br>
      </td>
      <td style="vertical-align: top; text-align: center;">1A<br>
      </td>
      <td style="vertical-align: top; text-align: center;">48<br>
      </td>
      <td style="vertical-align: top; text-align: center;">08<br>
      </td>
      <td style="vertical-align: top; text-align: center;">06<br>
      </td>
      <td style="vertical-align: top; text-align: center;">EC<br>
      </td>
      <td style="vertical-align: top; text-align: center;">00<br>
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
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Power level<br>
      <br>
      </span>09: ECO<br>
0A: Normal<br>
0B: Power<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">PAS</span><br>
      <br>
56: Push help<br>
40: PAS 0<br>
41: PAS 1<br>
42: PAS 2<br>
43: PAS 3<br>
44: PAS 4<br>
45: PAS 5<br>
46: PAS 6<br>
      <br>
      <span style="font-weight: bold;">Changes with display light on:</span><br>
      <br>
C2: PAS2<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Top speed</span><br>
      <br>
19: 25 Km/h<br>
1A: 36 Km/h<br>
      </td>
      <td style="vertical-align: top; text-align: center; font-weight: bold;">Wheel size (low byte)<br>
      <br>
48: 28"<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Wheel size (high byte)<br>
      <br>
      </span>08: 28"<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Thumb throttle</span><br>
      <br>
00: up to full speed<br>
06: up to 6 Km/h<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Checksum<br>
      </span>&nbsp;(B1+B2+B3+B4+B5+B6+B7) mod 256<br>
      </td>
      <td style="vertical-align: top; text-align: center;">?<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 0D<br>
      </span>&nbsp;(CR)<br>
      </td>
      <td style="vertical-align: top; text-align: center;"><span style="font-weight: bold;">Always 0A<br>
      </span>&nbsp;(LF)<br>
      </td>
    </tr>
  </tbody>
</table>
