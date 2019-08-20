Drupal.behaviors.formSingleSubmit = {
   attach: function() {
       function onFormSubmit(e) {
           if (e.isDefaultPrevented()) {
               // Don't act on form submissions that have been prevented by other JS.
               return;
           }
           var $form = jQuery(e.currentTarget);
           var formValues = $form.serialize();
           var previousValues = $form.attr('data-drupal-form-submit-last');
           if (previousValues === formValues) {
               e.preventDefault();
           }
           else {
               $form.attr('data-drupal-form-submit-last', formValues);
           }
       }

       jQuery('body').once('form-single-submit')
               .delegate('form:not([method~="GET"])', 'submit.singleSubmit', onFormSubmit);

   }
};