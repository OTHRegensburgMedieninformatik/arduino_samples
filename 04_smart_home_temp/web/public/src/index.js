function start() {
  $.ajax(
    {
      url: "http://localhost:3000/sensor/temp",
      dataType: "json"
    }
  ).done(dataReceived);
}

function dataReceived(data) {
  drawTemperaturesLine(data);
}

function drawTemperaturesLine(data) {
  new Morris.Line({
  // ID of the element in which to draw the chart.
  element: 'myfirstchart',
  // Chart data records -- each entry in this array corresponds to a point on
  // the chart.
  data: data,
  // The name of the data record attribute that contains x-values.
  xkey: 'timestamp',
  // A list of names of data record attributes that contain y-values.
  ykeys: ['temperature'],
  // Labels for the ykeys -- will be displayed when you hover over the
  // chart.
  labels: ['Value']
});
}
