(function () {

  function init(){
    $('#submitButton').click(submitButtonHandler);
  }

  function submitButtonHandler (evt) {
     var testForm = document.getElementById('testForm');

      evt.preventDefault();
	  evt.stopPropagation();

		$.ajax({
    		url: "/form",
    		type: "POST",
    		data: { id: testForm.id.value, tiempo: testForm.tiempo.value}
			}).done(function( e ) {
    			/*alert( "word was saved" + e );*/
		});
	}

//init on document ready
$(document).ready(init);
})();
